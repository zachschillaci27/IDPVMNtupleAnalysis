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
#include "IDPVMNtupleAnalysis/ResolutionHelper.h"

template <class H1, class H2> void CompareWithIDPVM(H1 hNtuple, H2* hIDPVM, const std::string & pname) {
    TCanvas *can = new TCanvas("CompareWithIDPVM", "", 800, 600);

    hNtuple->Draw();
    hNtuple->SetLineColor(kRed + 1);
    hNtuple->SetMarkerColor(kRed + 1);
    
    hIDPVM->Draw("SAME");
    hIDPVM->SetLineColor(kBlue + 1);
    hIDPVM->SetMarkerColor(kBlue + 1);

    std::vector<PlotUtils::LegendEntry> legendEntries = {
        PlotUtils::LegendEntry(hNtuple(), "Ntuple", "PL"),
        PlotUtils::LegendEntry(hIDPVM, "IDPVM", "PL")
    };

    PlotUtils::drawLegend(legendEntries, 0.75, 0.75, 0.90, 0.90);
    PlotUtils::saveCanvas(can, pname);
}

int main (int, char**){

    SetAtlasStyle();

    const std::string myphysval = "/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/Results/MyPhysVal.root";

    Sample<IDPVMTree> myPhysVal("", myphysval, "IDPerformanceMon/Ntuples/IDPerformanceMon_NtuplesTruthToReco");   

    Selection<IDPVMTree> hasTruth("hasTruth",[](IDPVMTree &t){return t.hasTruth(); });
    Selection<IDPVMTree> hasTrack("hasTrack",[](IDPVMTree &t){return t.hasTrack(); });
    Selection<IDPVMTree> matched("matched",[](IDPVMTree &t){return (t.track_truthMatchProb() > 0.5); });
    Selection<IDPVMTree> primary("primary",[](IDPVMTree &t){return (t.truth_barcode() < 200000 && t.truth_barcode() != 0); });
    Selection<IDPVMTree> matchedPrimaryTracks = hasTruth && hasTrack && matched;

    TH2D hPull_d0_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::d0, IDPVMDefs::eta);
    TH2D hPull_z0_vs_eta = IDPVMTemplates::getPullHistTemplate(IDPVMDefs::z0, IDPVMDefs::eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> d0Pull_eta ("d0Pull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_d0() - t.truth_d0())/t.trackErr_d0() );}, hPull_d0_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> z0Pull_eta ("z0Pull_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_z0() - t.truth_z0())/t.trackErr_z0() );}, hPull_z0_vs_eta);

    Plot<TH2D> d0PullTH2D(myPhysVal, matchedPrimaryTracks, d0Pull_eta);
    Plot<TH2D> z0PullTH2D(myPhysVal, matchedPrimaryTracks, z0Pull_eta);

    d0PullTH2D.populate();
    z0PullTH2D.populate();

    std::pair<Plot<TH1D>, Plot<TH1D>> d0Pulls = GetPulls(d0PullTH2D, IDPVMDefs::d0);
    std::pair<Plot<TH1D>, Plot<TH1D>> z0Pulls = GetPulls(z0PullTH2D, IDPVMDefs::z0);

    TH1* d0PullWidthIDPVM = ReadHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0pullresolutionRMS_vs_eta");
    TH1* z0PullWidthIDPVM = ReadHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0pullresolutionRMS_vs_eta");

    TH1* d0PullMeanIDPVM = ReadHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0pullmeanRMS_vs_eta");
    TH1* z0PullMeanIDPVM = ReadHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0pullmeanRMS_vs_eta");

    CompareWithIDPVM<Plot<TH1D>, TH1>(d0Pulls.first, d0PullWidthIDPVM, "d0PullWidth");
    CompareWithIDPVM<Plot<TH1D>, TH1>(z0Pulls.first, z0PullWidthIDPVM, "z0PullWidth");

    CompareWithIDPVM<Plot<TH1D>, TH1>(d0Pulls.second, d0PullMeanIDPVM, "d0PullMean");
    CompareWithIDPVM<Plot<TH1D>, TH1>(z0Pulls.second, z0PullMeanIDPVM, "z0PullMean");

    return 0;
}
