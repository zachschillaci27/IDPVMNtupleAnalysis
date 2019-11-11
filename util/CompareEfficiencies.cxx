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

    eff->GetYaxis()->SetRangeUser(0.75 , 1.05);
 
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

    const std::string myphysval = "/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/MyPhysVal.root";
    Sample<IDPVMTree> myPhysVal("", myphysval, "IDPerformanceMon/Ntuples/IDPerformanceMon_NtuplesTruthToReco");   

    Selection<IDPVMTree> selEfficiencyNum = IDPVMSelections::forEfficiencyNum();
    Selection<IDPVMTree> selEfficiencyDen = IDPVMSelections::forEfficiencyDen();

    TH1D h_eta = IDPVMTemplates::getEfficiencyHistTemplate(IDPVMDefs::eta);
    TH1D h_pt = IDPVMTemplates::getEfficiencyHistTemplate(IDPVMDefs::pt);

    PlotFillInstructionWithRef<TH1D, IDPVMTree> truthEta("truthEta", [](TH1D* h, IDPVMTree &t){ h->Fill(t.truth_eta());}, h_eta);
    PlotFillInstructionWithRef<TH1D, IDPVMTree> truthPt("truthPt", [](TH1D* h, IDPVMTree &t){ h->Fill(t.truth_pt() / 1000.);}, h_pt);

    Plot<TH1D> etaEffNum(myPhysVal, selEfficiencyNum, truthEta);
    Plot<TH1D> etaEffDen(myPhysVal, selEfficiencyDen, truthEta);

    Plot<TH1D> ptEffNum(myPhysVal, selEfficiencyNum, truthPt);
    Plot<TH1D> ptEffDen(myPhysVal, selEfficiencyDen, truthPt);

    etaEffNum.populate();
    etaEffDen.populate();

    ptEffNum.populate();
    ptEffDen.populate();

    Plot<TH1> etaEff = PlotUtils::getRatio(etaEffNum, etaEffDen, PlotUtils::efficiencyErrors);
    Plot<TH1> ptEff = PlotUtils::getRatio(ptEffNum, ptEffDen, PlotUtils::efficiencyErrors);

    Plot<TEfficiency> etaEffIDPVM = LoadIDPVMHistogram<TEfficiency>(myphysval, "IDPerformanceMon/Tracks/SelectedGoodTracks/trackeff_vs_eta");
    Plot<TEfficiency> ptEffIDPVM = LoadIDPVMHistogram<TEfficiency>(myphysval, "IDPerformanceMon/Tracks/SelectedGoodTracks/trackeff_vs_pt");

    CompareWithIDPVM(etaEffIDPVM, etaEff);
    CompareWithIDPVM(ptEffIDPVM, ptEff);

    return 0;
}
