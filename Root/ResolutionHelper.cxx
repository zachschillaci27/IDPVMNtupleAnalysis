#include "IDPVMNtupleAnalysis/ResolutionHelper.h"

ResOutput GetIterativeRMS(const Plot<TH2D> & hist2D, const unsigned int nbin, bool verbose) {
    //Get RMS of distribution using iterative RMS approach

    Plot<TH1D> hist1D("hist1D", hist2D->ProjectionY("hist1D", nbin, nbin));

    //Parameters to iterative RMS convergence technique
    // convergence
    bool converged = false;
    // iteration steps
    unsigned int nIters = 0;
    const unsigned int nMaxIters = 100;
    // width of cutting range in [RMS]
    const unsigned int nRMS_width = 3.0;

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

    // iteratively cut tails untill the RMS gets stable about mean
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

        if (verbose) {
            std::cout << "Iterative RMS ---> Bin: " << std::to_string(nbin) + ", Iteration: " + std::to_string(nIters) << ", RMS: " + std::to_string(RMS) << std::endl;
        }
    }

    if (!converged) {
        std::cout << "Iterative RMS procedure failed to converge, extracting RMS vis Gaussian fit ..." << std::endl;

        // evaluate mean and with via the Gauss fit
        TFitResultPtr fit = hist1D->Fit("gaus", "QS0");
        if (fit.Get() && (0 == (fit->Status() % 1000))) {
            mean = fit->Parameter(1);
            dMean = fit->ParError(1);
            RMS  = fit->Parameter(2);
            dRMS = fit->ParError(2);
        }
    }

    return ResOutput{mean, dMean, RMS, dRMS};
}

Plot<TH1D> GetResolution(const Plot<TH2D> & hist2D, IDPVMDefs::variable var) {
    //Method for generating resolution plots (as a Plot<TH1D>)

    TH1D *h_res = hist2D->ProjectionX("hist1D");
    h_res->Reset();
    h_res->GetYaxis()->SetTitle(IDPVMLabels::getResolutionLabel(var).c_str());

    for (unsigned int i = 1; i <= h_res->GetNbinsX(); ++i) {
        ResOutput results = GetIterativeRMS(hist2D, i);
        
        double units = 1.;
        if ((var == IDPVMDefs::d0) || (var == IDPVMDefs::z0)) units = 1000.;
        
        h_res->SetBinContent(i, units * results.RMS);
        h_res->SetBinError(i, units * results.dRMS);
    }
  
  return Plot<TH1D>("h_res", h_res, IDPVMLabels::getResolutionLabel(var), "PL");
}

std::pair<Plot<TH1D>, Plot<TH1D>> GetPulls(const Plot<TH2D> & hist2D, IDPVMDefs::variable var) {
    //Method for generating pull plots (as a Plot<TH1D>)

    TH1D *h_pullWidth = hist2D->ProjectionX("hist1D_pullWidth");
    h_pullWidth->Reset();
    h_pullWidth->GetYaxis()->SetTitle(IDPVMLabels::getPullWidthLabel(var).c_str());

    TH1D *h_pullMean = hist2D->ProjectionX("hist1D_pullMean");
    h_pullMean->Reset();
    h_pullMean->GetYaxis()->SetTitle(IDPVMLabels::getPullMeanLabel(var).c_str());

    for (unsigned int i = 1; i <= h_pullWidth->GetNbinsX(); ++i) {
        ResOutput results = GetIterativeRMS(hist2D, i);

        h_pullWidth->SetBinContent(i, results.RMS);
        h_pullWidth->SetBinError(i, results.dRMS);

        h_pullMean->SetBinContent(i, results.mean);
        h_pullMean->SetBinError(i, results.dMean);
    }
  
  return std::make_pair(Plot<TH1D>("h_pullWidth", h_pullWidth, IDPVMLabels::getPullWidthLabel(var), "PL"), 
                        Plot<TH1D>("h_pullMean", h_pullMean, IDPVMLabels::getPullMeanLabel(var), "PL"));
}