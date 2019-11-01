#include "NtupleAnalysisUtils/NtupleBranch.h"
#include "NtupleAnalysisUtils/Selection.h"
#include "NtupleAnalysisUtils/PlotFillInstruction.h"
#include "NtupleAnalysisUtils/PlotContent.h"
#include "NtupleAnalysisUtils/Sample.h"
#include "NtupleAnalysisUtils/PlotUtils.h"
#include "NtupleAnalysisUtils/CanvasOptions.h"
#include "NtupleAnalysisUtils/AtlasStyle.h"
#include "NtupleAnalysisUtils/DefaultPlotting.h"
#include "IDPVMNtupleAnalysis/IDPVMTree.h"
#include "IDPVMNtupleAnalysis/IDPVMTemplates.h"
#include <utility>
#include "TFitResultPtr.h"
#include "TFitResult.h"

std::pair<double, double> GetIterativeRMS(const Plot<TH2D> & histo2D, const unsigned int nbin) {
    //Get RMS of distribution using iterative RMS approach

    Plot<TH1D> hist1D("hist1D", histo2D->ProjectionY("hist1D", nbin, nbin));

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

    // evaluate  mean and RMS using iterative convergence:
    double mean = hist1D->GetMean();
    double RMS = hist1D->GetRMS();
    double dRMS = hist1D->GetRMSError();

    // iteratively cut tails untill the RMS gets stable about mean
    // RMS stable: when input histogram range after cutting by 
    // +- 3*RMS is same as the range before cutting
    while (nIters < nMaxIters) {    
        ++nIters;

        mean = hist1D->GetMean();
        RMS = hist1D->GetRMS();
        dRMS = hist1D->GetRMSError();

        xmin = -1.0 * nRMS_width * RMS + mean;
        xmax = nRMS_width * RMS + mean;
      
        // find bins corresponding to new range, disregard underflow
        int binmin = std::max(1, hist1D->GetXaxis()->FindFixBin(xmin));
        // find bins corresponding to new range, disregard overflow
        int binmax = std::min(hist1D->GetNbinsX(), hist1D->GetXaxis()->FindFixBin(xmax));
      
        // end iteration if these are same bins as in prev. iteration
        if ((binmin_was == binmin) && (binmax_was == binmax)) {
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

    if (!converged) {
        std::cout << "Iterative RMS procedure failed to converge, extracting RMS vis Gaussian fit ..." << std::endl;

        // evaluate mean and with via the Gauss fit
        TFitResultPtr fit = hist1D->Fit("gaus", "QS0");
        if (fit.Get() && (0 == (fit->Status() % 1000))) {
            mean = fit->Parameter(1);
            RMS  = fit->Parameter(2);
            dRMS = fit->ParError(2);
        }
    }

    return std::make_pair(RMS, dRMS);
}

Plot<TH1D> GetResolution(const Plot<TH2D> & histo2D, IDPVMDefs::variable var) {
    //Method for generating resolution plots (as a TH1D)

    TH1D *resolution = histo2D->ProjectionX("hist1D");
    resolution->Reset();

    resolution->GetYaxis()->SetTitle(IDPVMLabels::getResolutionLabel(var).c_str());

    for (unsigned int i = 1; i <= resolution->GetNbinsX(); ++i) {
        std::pair<double, double> results = GetIterativeRMS(histo2D, i);
        
        double units = 1.;
        if ((var == IDPVMDefs::d0) || (var == IDPVMDefs::z0)) units = 1000.;
        
        resolution->SetBinContent(i, units * results.first);
        resolution->SetBinError(i, units * results.second);
    }
  
  return Plot<TH1D>("resolution", resolution);
}

void ExtractAndDrawResolution(const Plot<TH2D> & histo2D, IDPVMDefs::variable var, const std::string & name) {
    PlotContent<TH1D> plotResolution({GetResolution(histo2D, var)}, {""}, name, "", CanvasOptions().logY(true));
    DefaultPlotting::draw1DNoRatio(plotResolution);
}

void DumpResults(const std::string & fname) {

    Sample<IDPVMTree> myPhysVal("", fname, "IDPerformanceMon/Ntuples/IDPerformanceMon_NtuplesTruthToReco");   

    Selection<IDPVMTree> hasTruth("hasTruth",[](IDPVMTree &t){return t.hasTruth(); });
    Selection<IDPVMTree> hasTrack("hasTrack",[](IDPVMTree &t){return t.hasTrack(); });
    Selection<IDPVMTree> matched("matched",[](IDPVMTree &t){return (t.track_truthMatchProb() > 0.5); });
    Selection<IDPVMTree> primary("primary",[](IDPVMTree &t){return (t.truth_barcode() < 200000 && t.truth_barcode() != 0); });
    Selection<IDPVMTree> matchedPrimaryTracks = hasTruth && hasTrack && matched && primary;

    TH2D hRes_d0_vs_eta = IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::d0, IDPVMDefs::eta);
    TH2D hRes_z0_vs_eta = IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::z0, IDPVMDefs::eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> d0Resolution_eta ("d0Resolution_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_d0() - t.truth_d0()));}, hRes_d0_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> z0Resolution_eta ("z0Resolution_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_z0() - t.truth_z0()));}, hRes_z0_vs_eta);

    Plot<TH2D> d0ResTH2D(myPhysVal, matchedPrimaryTracks, d0Resolution_eta);
    Plot<TH2D> z0ResTH2D(myPhysVal, matchedPrimaryTracks, z0Resolution_eta);

    d0ResTH2D.populate();
    z0ResTH2D.populate();

    ExtractAndDrawResolution(d0ResTH2D, IDPVMDefs::d0, "IDPVM/d0Resolution_vs_eta");
    ExtractAndDrawResolution(z0ResTH2D, IDPVMDefs::z0, "IDPVM/z0Resolution_vs_eta");

}

int main (int, char**){

    SetAtlasStyle();

    DumpResults("/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/Results/MyPhysVal.root");

    return 0;
}
