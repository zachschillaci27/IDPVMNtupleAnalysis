#include "IDPVMNtupleAnalysis/ResolutionHelper.h"

ResolOutput GetGaussFit(const Plot<TH2D> & hist2D, const unsigned int nbin) {
    //Get resolution of distribution via a Gaussian fit

    const std::string hname = hist2D.getName() + std::string("py_bin") + std::to_string(nbin);
    Plot<TH1D> hist1D("hist1D", hist2D->ProjectionY(hname.c_str(), nbin, nbin));

    double mean = hist1D->GetMean();
    double dMean = hist1D->GetMeanError();
    double RMS = hist1D->GetRMS();
    double dRMS = hist1D->GetRMSError();

    // evaluate mean and RMS from Gaussian fit
    TFitResultPtr fit = hist1D->Fit("gaus", "QS0");
    if (fit.Get() && (0 == (fit->Status() % 1000))) {
        mean = fit->Parameter(1);
        dMean = fit->ParError(1);
        RMS  = fit->Parameter(2);
        dRMS = fit->ParError(2);
    }

    return ResolOutput{mean, dMean, RMS, dRMS, true};
}

ResolOutput GetIterativeRMS(const Plot<TH2D> & hist2D, const unsigned int nbin, const bool save) {
    //Get RMS of distribution using iterative RMS approach

    const std::string hname = hist2D.getName() + std::string("py_bin") + std::to_string(nbin);
    Plot<TH1D> hist1D("hist1D", hist2D->ProjectionY(hname.c_str(), nbin, nbin));

    //Parameters to iterative RMS convergence technique
    // convergence
    bool converged = false;
    // iteration steps
    unsigned int nIters = 0;
    const unsigned int nMaxIters = 100;
    // width of cutting range in [RMS]
    const double nRMS_width = 3.0;

    // min and max range of the histogram:
    double xmin = 0.;
    double xmax = 0.;
    // min and max bins of the histogram in previous iteration
    int binmin_was = 1;
    int binmax_was = hist1D->GetNbinsX();

    // evaluate mean and RMS using iterative convergence:
    double mean = hist1D->GetMean();
    double dMean = hist1D->GetMeanError();
    double RMS = hist1D->GetRMS();
    double dRMS = hist1D->GetRMSError();

    // iteratively cut tails until the RMS gets stable about mean
    // RMS stable: when input histogram range after cutting by 
    // +- 3*RMS is same as the range before cutting
    while (nIters < nMaxIters) {    
        ++nIters;

        mean = hist1D->GetMean();
        dMean = hist1D->GetMeanError();
        RMS = hist1D->GetRMS();
        dRMS = hist1D->GetRMSError();

        xmin = -1.0 * nRMS_width * RMS + mean;
        xmax = nRMS_width * RMS + mean;
      
        // find bins corresponding to new range, disregard underflow
        int binmin = std::max(1, hist1D->GetXaxis()->FindFixBin(xmin));
        // find bins corresponding to new range, disregard overflow
        int binmax = std::min(hist1D->GetNbinsX(), hist1D->GetXaxis()->FindFixBin(xmax));
        // end iteration if these are same bins as in prev. iteration
        if ((binmin == binmin_was) && (binmax == binmax_was)) {
            converged = true;
	        break;
        }
        else {
	        // else set the new range and continue iteration 
	        hist1D->GetXaxis()->SetRange(binmin, binmax);
	        binmin_was = binmin;
	        binmax_was = binmax;
        }
    }

    if (save) {
        TCanvas *can = new TCanvas();
        gStyle->SetOptStat(1);
        hist1D->Draw();
        hist1D->GetXaxis()->SetRangeUser(hist1D->GetXaxis()->GetBinLowEdge(1), hist1D->GetXaxis()->GetBinUpEdge(hist1D->GetXaxis()->GetNbins()));
        hist1D()->SetStats(kTRUE);
        PlotUtils::saveCanvas(can, hname);
        gStyle->SetOptStat(0);
        delete can;
    }

    return ResolOutput{mean, dMean, RMS, dRMS, converged};
}

Plot<TH1> GetResolution(const Plot<TH2D> & hist2D, IDPVMDefs::variable var, const bool save) {
    //Method for generating resolution plots (as a Plot<TH1>)

    TH1 *h_res = hist2D->ProjectionX("hist1D");
    h_res->Reset();
    h_res->GetYaxis()->SetTitle(IDPVMLabels::getResolutionLabel(var).c_str());

    double units = ((var == IDPVMDefs::d0) || (var == IDPVMDefs::z0) || (var == IDPVMDefs::z0sin)) ? 1000. : 1.;

    for (int i = 1; i < h_res->GetNbinsX() + 1; ++i) {
        ResolOutput results = GetIterativeRMS(hist2D, i, save);

        h_res->SetBinContent(i, units * results.RMS);
        h_res->SetBinError(i, units * results.dRMS);
    }

    if (save) {
        TCanvas *can = new TCanvas();
        gStyle->SetOptStat(1);
        hist2D->Draw();
        hist2D->GetXaxis()->SetRangeUser(hist2D->GetXaxis()->GetBinLowEdge(1), hist2D->GetXaxis()->GetBinUpEdge(hist2D->GetXaxis()->GetNbins()));
        hist2D->GetYaxis()->SetRangeUser(hist2D->GetYaxis()->GetBinLowEdge(1), hist2D->GetYaxis()->GetBinUpEdge(hist2D->GetYaxis()->GetNbins()));
        hist2D()->SetMarkerSize(0.1);
        hist2D()->SetStats(kTRUE);
        PlotUtils::saveCanvas(can, hist2D.getName());
        gStyle->SetOptStat(0);
        delete can;
    }
    
    Plot<TH1> theRes(hist2D.getName() + "-ResolutionPlot", h_res);
    delete h_res;

    return theRes;
}

std::pair<Plot<TH1>, Plot<TH1>> GetPulls(const Plot<TH2D> & hist2D, IDPVMDefs::variable var) {
    //Method for generating pull plots (as a Plot<TH1>)

    TH1 *h_pullWidth = hist2D->ProjectionX("hist1D_pullWidth");
    h_pullWidth->Reset();
    h_pullWidth->GetYaxis()->SetTitle(IDPVMLabels::getPullWidthLabel(var).c_str());

    TH1 *h_pullMean = hist2D->ProjectionX("hist1D_pullMean");
    h_pullMean->Reset();
    h_pullMean->GetYaxis()->SetTitle(IDPVMLabels::getPullMeanLabel(var).c_str());

    for (int i = 1; i < h_pullWidth->GetNbinsX() + 1; ++i) {
        ResolOutput results = GetIterativeRMS(hist2D, i);

        h_pullWidth->SetBinContent(i, results.RMS);
        h_pullWidth->SetBinError(i, results.dRMS);

        h_pullMean->SetBinContent(i, results.mean);
        h_pullMean->SetBinError(i, results.dMean);
    }

    std::pair<Plot<TH1>, Plot<TH1>> thePulls = std::make_pair(Plot<TH1>(hist2D.getName() + "-PullWidthPlot", h_pullWidth), 
                                                              Plot<TH1>(hist2D.getName() + "-PullMeanPlot", h_pullMean));
    delete h_pullWidth;
    delete h_pullMean;

    return thePulls;
}