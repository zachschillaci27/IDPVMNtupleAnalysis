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

void CompareWithIDPVM(Plot<TProfile> effIDPVM, Plot<TH1> eff) {
    TCanvas *can = new TCanvas("CompareWithIDPVM", "", 800, 600);

    eff->SetMarkerStyle(kOpenCircle);
    eff->SetMarkerColor(kBlue + 1);
    eff->SetLineColor(kBlue + 1);
    eff->Draw();
 
    effIDPVM->SetMarkerStyle(kFullDotLarge);
    effIDPVM->SetMarkerColor(kRed + 1);
    effIDPVM->SetLineColor(kRed + 1);
    effIDPVM->Draw("SAME");

    std::vector<PlotUtils::LegendEntry> legendEntries = {
        PlotUtils::LegendEntry(effIDPVM(), "IDPVM", "PL"),
        PlotUtils::LegendEntry(eff(), "Ntuple", "PL")
    };

    PlotUtils::drawLegend(legendEntries, 0.75, 0.75, 0.90, 0.90);

    PlotUtils::saveCanvas(can, effIDPVM.getName());
}

int main (int, char**) {

    SetAtlasStyle();

    const std::string myphysval = "/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/Results/ttbar/MyPhysVal.root";
    Sample<IDPVMTree> myPhysVal("", myphysval, "IDPerformanceMon/Ntuples/IDPerformanceMon_NtuplesTruthToReco");   

    Selection<IDPVMTree> selFakeRateNum = IDPVMSelections::forFakeRateNum();
    Selection<IDPVMTree> selFakeRateDen = IDPVMSelections::forFakeRateDen();

    TH1D h_eta = IDPVMTemplates::getFakeRateHistTemplate(IDPVMDefs::eta);
    TH1D h_pt = IDPVMTemplates::getFakeRateHistTemplate(IDPVMDefs::pt);

    PlotFillInstructionWithRef<TH1D, IDPVMTree> trackEta("trackEta", [](TH1D* h, IDPVMTree &t){ h->Fill(t.track_eta());}, h_eta);
    PlotFillInstructionWithRef<TH1D, IDPVMTree> trackPt("trackPt", [](TH1D* h, IDPVMTree &t){ h->Fill(t.track_pt() / 1000.);}, h_pt);

    Plot<TH1D> etaEffNum(myPhysVal, selFakeRateNum, trackEta);
    Plot<TH1D> etaEffDen(myPhysVal, selFakeRateDen, trackEta);

    Plot<TH1D> ptEffNum(myPhysVal, selFakeRateNum, trackPt);
    Plot<TH1D> ptEffDen(myPhysVal, selFakeRateDen, trackPt);

    etaEffNum.populate();
    etaEffDen.populate();

    ptEffNum.populate();
    ptEffDen.populate();

    Plot<TH1> etaEff = PlotUtils::getRatio(etaEffNum, etaEffDen, PlotUtils::efficiencyErrors);
    Plot<TH1> ptEff = PlotUtils::getRatio(ptEffNum, ptEffDen, PlotUtils::efficiencyErrors);

    Plot<TProfile> etaEffIDPVM = LoadIDPVMHistogram<TProfile>(myphysval, "IDPerformanceMon/Tracks/SelectedFakeTracks/track_fakerate_vs_eta");
    Plot<TProfile> ptEffIDPVM = LoadIDPVMHistogram<TProfile>(myphysval, "IDPerformanceMon/Tracks/SelectedFakeTracks/track_fakerate_vs_pt");

    CompareWithIDPVM(etaEffIDPVM, etaEff);
    CompareWithIDPVM(ptEffIDPVM, ptEff);

    return 0;
}
