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
#include "IDPVMNtupleAnalysis/IDPVMSelections.h"
#include <utility>

void DumpResults(const std::string & fname){

    Sample<IDPVMTree> ntuple("Ntuple", fname, "SquirrelPlots/Ntuples/SquirrelPlots_NtuplesTruthToReco");   
        
    Selection<IDPVMTree> hasTruth = IDPVMSelections::hasTruth();
    Selection<IDPVMTree> hasTrack = IDPVMSelections::hasTrack();

    TH1D h_eta = IDPVMTemplates::getEfficiencyHistTemplate(IDPVMDefs::eta);
    TH1D h_pt= IDPVMTemplates::getEfficiencyHistTemplate(IDPVMDefs::pt);

    PlotFillInstructionWithRef<TH1D, IDPVMTree> truthEta("truthEta", [](TH1D* h, IDPVMTree &t){ h->Fill(t.truth_eta());}, h_eta);
    PlotFillInstructionWithRef<TH1D, IDPVMTree> trackEta("trackEta", [](TH1D* h, IDPVMTree &t){ h->Fill(t.track_eta());}, h_eta);
 
    PlotFillInstructionWithRef<TH1D, IDPVMTree> truthPt("truthPt", [](TH1D* h, IDPVMTree &t){ h->Fill(t.truth_pt() * 0.001);}, h_pt);
    PlotFillInstructionWithRef<TH1D, IDPVMTree> trackPt("trackPt", [](TH1D* h, IDPVMTree &t){ h->Fill(t.track_pt() * 0.001);}, h_pt);

    PlotContent<TH1D> plot_yields_eta({Plot<TH1D> (ntuple, hasTruth, truthEta, "Truth eta", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kRed)),
                                       Plot<TH1D> (ntuple, hasTrack, trackEta, "Track eta", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kBlue))},
                                       {""}, "IDPVM/Track_vs_Truth_eta","", CanvasOptions().labelLumiTag("HL-LHC").labelSqrtsTag("14 TeV").ratioAxisTitle("vs. Truth"));

    PlotContent<TH1D> plot_yields_pt({Plot<TH1D> (ntuple, hasTruth, truthPt, "Truth pt", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kRed)),
                                      Plot<TH1D> (ntuple, hasTrack, trackPt, "Track pt", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kBlue))},
                                      {""}, "IDPVM/Track_vs_Truth_pt","", CanvasOptions().labelLumiTag("HL-LHC").labelSqrtsTag("14 TeV").ratioAxisTitle("vs. Truth"));

    DefaultPlotting::draw1DWithRatio(plot_yields_eta);
    DefaultPlotting::draw1DWithRatio(plot_yields_pt);
}

int main (int, char**){

    SetAtlasStyle();

    DumpResults("/Users/zschillaci/CERN/Working/IDPVMAnalysis/run/PHYSVAL.CL18.root");

    return 0;
}
