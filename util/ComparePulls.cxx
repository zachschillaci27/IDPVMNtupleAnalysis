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

void CompareWithIDPVM(std::vector<std::pair<Plot<TH1>, Plot<TH1>>> thePlotPairs, const std::vector<std::string> & labels) {  
    for (auto & thePair : thePlotPairs) {
        thePair.first.setLegendTitle("IDPVM");
        thePair.first.setLegendOption("PL");
        thePair.first.setPlotFormat(PlotFormat().MarkerStyle(kFullDotLarge).MarkerColor(kRed + 1).LineColor(kRed + 1));
        
        thePair.second.setLegendTitle("Ntuple");
        thePair.second.setLegendOption("PL");
        thePair.second.setPlotFormat(PlotFormat().MarkerStyle(kOpenCircle).MarkerColor(kBlue + 1).LineColor(kBlue + 1));

        PlotContent<TH1> theContent({thePair.first, thePair.second}, labels, thePair.first.getName(), "",
                                     CanvasOptions().labelLumiTag("HL-LHC").labelSqrtsTag("14 TeV").yAxisTitle(thePair.second->GetYaxis()->GetTitle()).ratioAxisTitle("Ntuple/IDPVM"));
        DefaultPlotting::draw1DWithRatio(theContent);
    }
}

int main (int, char**) {

    SetAtlasStyle();

    const std::string location = "/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/sglmu100/MyPhysVal.root";
    Sample<IDPVMTree> myPhysVal("", location, "IDPerformanceMon/Ntuples/IDPerformanceMon_NtuplesTruthToReco");   

    Selection<IDPVMTree> selResolutions = IDPVMSelections::forResolution();

    TH2D hPull_pt_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::pt, IDPVMDefs::eta);
    TH2D hPull_d0_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::d0, IDPVMDefs::eta);
    TH2D hPull_z0_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::z0, IDPVMDefs::eta);
    TH2D hPull_z0sin_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::z0sin, IDPVMDefs::eta);
    TH2D hPull_phi_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::phi, IDPVMDefs::eta);
    TH2D hPull_theta_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::theta, IDPVMDefs::eta);
    TH2D hPull_qOverPt_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::qOverPt, IDPVMDefs::eta);
    TH2D hPull_qOverP_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::qOverP, IDPVMDefs::eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> ptPull_eta ("ptPull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_pt() - t.truth_pt())/t.trackErr_pt());}, hPull_pt_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> d0Pull_eta ("d0Pull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_d0() - t.truth_d0())/t.trackErr_d0());}, hPull_d0_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> z0Pull_eta ("z0Pull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_z0() - t.truth_z0())/t.trackErr_z0());}, hPull_z0_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> z0sinPull_eta ("z0sinPull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_z0sin() - t.truth_z0sin())/t.trackErr_z0sin());}, hPull_z0sin_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> phiPull_eta ("phiPull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_phi() - t.truth_phi())/t.trackErr_phi());}, hPull_phi_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> thetaPull_eta ("thetaPull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_theta() - t.truth_theta())/t.trackErr_theta());}, hPull_theta_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> qOverPtPull_eta ("qOverPtPull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_qOverPt() - t.truth_qOverPt())/t.trackErr_qOverPt());}, hPull_qOverPt_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> qOverPPull_eta ("qOverPPull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_qOverP() - t.truth_qOverP())/t.trackErr_qOverP());}, hPull_qOverP_vs_eta);

    Plot<TH2D> ptPullTH2D(myPhysVal, selResolutions, ptPull_eta);
    Plot<TH2D> d0PullTH2D(myPhysVal, selResolutions, d0Pull_eta);
    Plot<TH2D> z0PullTH2D(myPhysVal, selResolutions, z0Pull_eta);
    Plot<TH2D> z0sinPullTH2D(myPhysVal, selResolutions, z0sinPull_eta);
    Plot<TH2D> phiPullTH2D(myPhysVal, selResolutions, phiPull_eta);
    Plot<TH2D> thetaPullTH2D(myPhysVal, selResolutions, thetaPull_eta);
    Plot<TH2D> qOverPtPullTH2D(myPhysVal, selResolutions, qOverPtPull_eta);
    Plot<TH2D> qOverPPullTH2D(myPhysVal, selResolutions, qOverPPull_eta);

    ptPullTH2D.populate();
    d0PullTH2D.populate();
    z0PullTH2D.populate();
    z0sinPullTH2D.populate();
    phiPullTH2D.populate();
    thetaPullTH2D.populate();
    qOverPtPullTH2D.populate();
    qOverPPullTH2D.populate();

    std::pair<Plot<TH1>, Plot<TH1>> ptPulls = GetPulls(ptPullTH2D, IDPVMDefs::pt);
    std::pair<Plot<TH1>, Plot<TH1>> d0Pulls = GetPulls(d0PullTH2D, IDPVMDefs::d0);
    std::pair<Plot<TH1>, Plot<TH1>> z0Pulls = GetPulls(z0PullTH2D, IDPVMDefs::z0);
    std::pair<Plot<TH1>, Plot<TH1>> z0sinPulls = GetPulls(z0PullTH2D, IDPVMDefs::z0sin);
    std::pair<Plot<TH1>, Plot<TH1>> phiPulls = GetPulls(phiPullTH2D, IDPVMDefs::phi);
    std::pair<Plot<TH1>, Plot<TH1>> thetaPulls = GetPulls(thetaPullTH2D, IDPVMDefs::theta);
    std::pair<Plot<TH1>, Plot<TH1>> qOverPtPulls = GetPulls(qOverPtPullTH2D, IDPVMDefs::qOverPt);
    std::pair<Plot<TH1>, Plot<TH1>> qOverPPulls = GetPulls(qOverPtPullTH2D, IDPVMDefs::qOverP);

    Plot<TH1> ptPullWidthIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/ptpullresolutionRMS_vs_eta");
    Plot<TH1> d0PullWidthIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0pullresolutionRMS_vs_eta");
    Plot<TH1> z0PullWidthIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0pullresolutionRMS_vs_eta");
    Plot<TH1> z0sinPullWidthIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0sinpullresolutionRMS_vs_eta");
    Plot<TH1> phiPullWidthIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/phipullresolutionRMS_vs_eta");
    Plot<TH1> thetaPullWidthIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/thetapullresolutionRMS_vs_eta");
    Plot<TH1> qOverPtPullWidthIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverptpullresolutionRMS_vs_eta");
    Plot<TH1> qOverPPullWidthIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverppullresolutionRMS_vs_eta");

    Plot<TH1> ptPullMeanIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/ptpullmeanRMS_vs_eta");
    Plot<TH1> d0PullMeanIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0pullmeanRMS_vs_eta");
    Plot<TH1> z0PullMeanIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0pullmeanRMS_vs_eta");
    Plot<TH1> z0sinPullMeanIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0sinpullmeanRMS_vs_eta");
    Plot<TH1> phiPullMeanIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/phipullmeanRMS_vs_eta");
    Plot<TH1> thetaPullMeanIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/thetapullmeanRMS_vs_eta");
    Plot<TH1> qOverPtPullMeanIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverptpullmeanRMS_vs_eta");
    Plot<TH1> qOverPPullMeanIDPVM = LoadIDPVMHistogram<TH1>(location, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverppullmeanRMS_vs_eta");

    std::vector<std::pair<Plot<TH1>, Plot<TH1>>> PullWidths = {
        std::make_pair(ptPullWidthIDPVM, ptPulls.first),
        std::make_pair(d0PullWidthIDPVM, d0Pulls.first),
        std::make_pair(z0PullWidthIDPVM, z0Pulls.first),
        std::make_pair(z0sinPullWidthIDPVM, z0sinPulls.first),
        std::make_pair(phiPullWidthIDPVM, phiPulls.first),
        std::make_pair(thetaPullWidthIDPVM, thetaPulls.first),  
        std::make_pair(qOverPtPullWidthIDPVM, qOverPtPulls.first),  
        std::make_pair(qOverPPullWidthIDPVM, qOverPPulls.first),  
    };

    std::vector<std::pair<Plot<TH1>, Plot<TH1>>> PullMeans = {
        std::make_pair(ptPullMeanIDPVM, ptPulls.second),
        std::make_pair(d0PullMeanIDPVM, d0Pulls.second),
        std::make_pair(z0PullMeanIDPVM, z0Pulls.second),
        std::make_pair(z0sinPullMeanIDPVM, z0sinPulls.second),
        std::make_pair(phiPullMeanIDPVM, phiPulls.second),
        std::make_pair(thetaPullMeanIDPVM, thetaPulls.second),  
        std::make_pair(qOverPtPullMeanIDPVM, qOverPtPulls.second),  
        std::make_pair(qOverPPullMeanIDPVM, qOverPPulls.second),  
    };

    CompareWithIDPVM(PullWidths, {"IDPVM Ntuple Validation", "Single #mu, 100 GeV"});
    CompareWithIDPVM(PullMeans, {"IDPVM Ntuple Validation", "Single #mu, 100 GeV"});

    return 0;
}
