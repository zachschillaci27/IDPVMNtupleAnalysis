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

void CompareWithIDPVM(Plot<TEfficiency> effIDPVM, Plot<TH1> eff) {
    TCanvas *can = new TCanvas("CompareWithIDPVM", "", 800, 600);

    eff->SetMarkerStyle(kOpenCircle);
    eff->SetMarkerColor(kBlue + 1);
    eff->SetLineColor(kBlue + 1);
    eff->Draw();

    eff->GetYaxis()->SetRangeUser(0.985, 1.015);
 
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

    const std::string myphysval = "/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/Results/MyPhysVal.root";
    Sample<IDPVMTree> myPhysVal("", myphysval, "IDPerformanceMon/Ntuples/IDPerformanceMon_NtuplesTruthToReco");   

    Selection<IDPVMTree> hasTruth = IDPVMSelections::hasTruth();
    Selection<IDPVMTree> passedTruth = IDPVMSelections::forEfficiencyNum();

    TH1D h_eta = IDPVMTemplates::getEfficiencyHistTemplate(IDPVMDefs::eta);

    PlotFillInstructionWithRef<TH1D, IDPVMTree> truthEta("truthEta", [](TH1D* h, IDPVMTree &t){ h->Fill(t.truth_eta());}, h_eta);
    PlotFillInstructionWithRef<TH1D, IDPVMTree> trackEta("trackEta", [](TH1D* h, IDPVMTree &t){ h->Fill(t.track_eta());}, h_eta);

    Plot<TH1D> etaEffNum(myPhysVal, passedTruth, truthEta, "All", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kBlue));
    Plot<TH1D> etaEffDen(myPhysVal, hasTruth, truthEta, "All", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kBlue));

    etaEffNum.populate();
    etaEffDen.populate();

    Plot<TH1> etaEff = PlotUtils::getRatio(etaEffNum, etaEffDen, PlotUtils::efficiencyErrors);

    Plot<TEfficiency> etaEffIDPVM = LoadIDPVMHistogram<TEfficiency>(myphysval, "IDPerformanceMon/Tracks/SelectedGoodTracks/trackeff_vs_eta");
    
    CompareWithIDPVM(etaEffIDPVM, etaEff);

    return 0;
}
