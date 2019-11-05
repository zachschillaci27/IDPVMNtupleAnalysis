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
#include "IDPVMNtupleAnalysis/IDPVMUtilities.h"
#include "IDPVMNtupleAnalysis/IDPVMSelections.h"
#include "IDPVMNtupleAnalysis/ResolutionHelper.h"

void CompareWithIDPVM(std::vector<std::pair<Plot<TH1>, Plot<TH1>>> thePlotPairs) {    
    for (auto & thePair : thePlotPairs) {
        thePair.first.setLegendTitle("IDPVM");
        thePair.first.setLegendOption("PL");
        thePair.first.setPlotFormat(PlotFormat().MarkerStyle(kFullDotLarge).MarkerColor(kRed + 1).LineColor(kRed + 1));
        
        thePair.second.setLegendTitle("Ntuple");
        thePair.second.setLegendOption("PL");
        thePair.second.setPlotFormat(PlotFormat().MarkerStyle(kOpenCircle).MarkerColor(kBlue + 1).LineColor(kBlue + 1));

        PlotContent<TH1> theContent({thePair.first, thePair.second}, {"ITk Step 3.0", "IDPVM Ntuple Validation"}, thePair.first.getName(), "",
                                     CanvasOptions().ratioAxisTitle("w.r.t. IDPVM"));
        DefaultPlotting::draw1DWithRatio(theContent);
    }
}

int main (int, char**) {

    SetAtlasStyle();

    const std::string myphysval = "/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/Results/MyPhysVal.root";
    Sample<IDPVMTree> myPhysVal("", myphysval, "IDPerformanceMon/Ntuples/IDPerformanceMon_NtuplesTruthToReco");   

    Selection<IDPVMTree> matchedPrimaryTracks = IDPVMSelections::isMatchedPrimary();

    TH2D hPull_d0_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::d0, IDPVMDefs::eta);
    TH2D hPull_z0_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::z0, IDPVMDefs::eta);
    TH2D hPull_phi_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::phi, IDPVMDefs::eta);
    TH2D hPull_theta_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::theta, IDPVMDefs::eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> d0Pull_eta ("d0Pull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_d0() - t.truth_d0())/t.trackErr_d0() );}, hPull_d0_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> z0Pull_eta ("z0Pull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_z0() - t.truth_z0())/t.trackErr_z0() );}, hPull_z0_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> phiPull_eta ("phiPull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_phi() - t.truth_phi())/t.trackErr_phi() );}, hPull_phi_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> thetaPull_eta ("thetaPull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_theta() - t.truth_theta())/t.trackErr_theta() );}, hPull_theta_vs_eta);

    Plot<TH2D> d0PullTH2D(myPhysVal, matchedPrimaryTracks, d0Pull_eta);
    Plot<TH2D> z0PullTH2D(myPhysVal, matchedPrimaryTracks, z0Pull_eta);
    Plot<TH2D> phiPullTH2D(myPhysVal, matchedPrimaryTracks, phiPull_eta);
    Plot<TH2D> thetaPullTH2D(myPhysVal, matchedPrimaryTracks, thetaPull_eta);

    d0PullTH2D.populate();
    z0PullTH2D.populate();
    phiPullTH2D.populate();
    thetaPullTH2D.populate();

    std::pair<Plot<TH1>, Plot<TH1>> d0Pulls = GetPulls(d0PullTH2D, IDPVMDefs::d0);
    std::pair<Plot<TH1>, Plot<TH1>> z0Pulls = GetPulls(z0PullTH2D, IDPVMDefs::z0);
    std::pair<Plot<TH1>, Plot<TH1>> phiPulls = GetPulls(phiPullTH2D, IDPVMDefs::phi);
    std::pair<Plot<TH1>, Plot<TH1>> thetaPulls = GetPulls(thetaPullTH2D, IDPVMDefs::theta);

    Plot<TH1> d0PullWidthIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0pullresolutionRMS_vs_eta");
    Plot<TH1> z0PullWidthIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0pullresolutionRMS_vs_eta");
    Plot<TH1> phiPullWidthIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/phipullresolutionRMS_vs_eta");
    Plot<TH1> thetaPullWidthIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/thetapullresolutionRMS_vs_eta");

    Plot<TH1> d0PullMeanIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0pullmeanRMS_vs_eta");
    Plot<TH1> z0PullMeanIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0pullmeanRMS_vs_eta");
    Plot<TH1> phiPullMeanIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/phipullmeanRMS_vs_eta");
    Plot<TH1> thetaPullMeanIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/thetapullmeanRMS_vs_eta");

    std::vector<std::pair<Plot<TH1>, Plot<TH1>>> PullWidths = {
        std::make_pair(d0PullWidthIDPVM, d0Pulls.first),
        std::make_pair(z0PullWidthIDPVM, z0Pulls.first),
        std::make_pair(phiPullWidthIDPVM, phiPulls.first),
        std::make_pair(thetaPullWidthIDPVM, thetaPulls.first),  
    };

    std::vector<std::pair<Plot<TH1>, Plot<TH1>>> PullMeans = {
        std::make_pair(d0PullMeanIDPVM, d0Pulls.second),
        std::make_pair(z0PullMeanIDPVM, z0Pulls.second),
        std::make_pair(phiPullMeanIDPVM, phiPulls.second),
        std::make_pair(thetaPullMeanIDPVM, thetaPulls.second),  
    };

    CompareWithIDPVM(PullWidths);
    CompareWithIDPVM(PullMeans);

    return 0;
}
