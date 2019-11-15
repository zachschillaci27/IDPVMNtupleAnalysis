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

    const std::string myphysval = "/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/ttbar/alternative/MyPhysVal.root";
    Sample<IDPVMTree> ntuple("", myphysval, "IDPerformanceMon/Ntuples/IDPerformanceMon_NtuplesTruthToReco");   

    Selection<IDPVMTree> selFakeRateNum = IDPVMSelections::forFakeRateNum();
    Selection<IDPVMTree> selFakeRateDen = IDPVMSelections::forFakeRateDen();

    TH1D h_eta = IDPVMTemplates::getFakeRateHistTemplate(IDPVMDefs::eta);
    TH1D h_pt = IDPVMTemplates::getFakeRateHistTemplate(IDPVMDefs::pt);

    PlotFillInstructionWithRef<TH1D, IDPVMTree> trackEta("trackEta", [](TH1D* h, IDPVMTree &t){ h->Fill(t.track_eta());}, h_eta);
    PlotFillInstructionWithRef<TH1D, IDPVMTree> trackPt("trackPt", [](TH1D* h, IDPVMTree &t){ h->Fill(t.track_pt() / 1000.);}, h_pt);

    Plot<TH1D> etaEffNum(ntuple, selFakeRateNum, trackEta);
    Plot<TH1D> etaEffDen(ntuple, selFakeRateDen, trackEta);

    Plot<TH1D> ptEffNum(ntuple, selFakeRateNum, trackPt);
    Plot<TH1D> ptEffDen(ntuple, selFakeRateDen, trackPt);

    etaEffNum.populate();
    etaEffDen.populate();

    ptEffNum.populate();
    ptEffDen.populate();

    Plot<TH1> etaEff = PlotUtils::getRatio(etaEffNum, etaEffDen, PlotUtils::efficiencyErrors);
    Plot<TH1> ptEff = PlotUtils::getRatio(ptEffNum, ptEffDen, PlotUtils::efficiencyErrors);

    Plot<TH1> etaEffIDPVM = CastIDPVMHistogram<TProfile, TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedFakeTracks/track_fakerate_vs_eta");
    Plot<TH1> ptEffIDPVM = CastIDPVMHistogram<TProfile, TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedFakeTracks/track_fakerate_vs_pt");

    std::vector<std::pair<Plot<TH1>, Plot<TH1>>> Efficiencies = {
        std::make_pair(etaEffIDPVM, etaEff),
        std::make_pair(ptEffIDPVM, ptEff),
    }; 

    CompareWithIDPVM(Efficiencies, {"IDPVM Ntuple Validation", "t#bar{t}"});

    return 0;
}
