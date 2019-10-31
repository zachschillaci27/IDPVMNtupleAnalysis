#include "NtupleAnalysisUtils/NtupleBranch.h"
#include "NtupleAnalysisUtils/Selection.h"
#include "NtupleAnalysisUtils/PlotFillInstruction.h"
#include "NtupleAnalysisUtils/PlotContent.h"
#include "NtupleAnalysisUtils/Sample.h"
#include "NtupleAnalysisUtils/PlotUtils.h"
#include "NtupleAnalysisUtils/CanvasOptions.h"
#include "NtupleAnalysisUtils/AtlasStyle.h"
#include "TrackingPerfPlots/IDPVMTree.h"
#include "NtupleAnalysisUtils/DefaultPlotting.h"

void DumpResults(const std::string & fname){

    Sample<IDPVMTree> mySample ("");
    mySample.addFile(fname,"IDPerformanceMon/Ntuples/IDPerformanceMon_NtuplesTruthToReco");   

    Selection<IDPVMTree> hasTruth("hasTruth",[](IDPVMTree &t){return t.hasTruth(); });
    Selection<IDPVMTree> hasTrack("hasTrack",[](IDPVMTree &t){return t.hasTrack(); });
    Selection<IDPVMTree> isUnassociated("isUnassociated",[](IDPVMTree &t){return t.truth_unassociated(); });
    Selection<IDPVMTree> isFake("isFake",[](IDPVMTree &t){return t.track_isFake(); });
    Selection<IDPVMTree> highProb("highProb",[](IDPVMTree &t){return (t.track_truthMatchProb() > 0.5); });
    Selection<IDPVMTree> primary("primary",[](IDPVMTree &t){return (t.truth_barcode() < 200000 && t.truth_barcode() != 0); });
    
    Selection<IDPVMTree> passedTrack = hasTrack && highProb;
    Selection<IDPVMTree> passedTruth = hasTruth && !isUnassociated && primary;

    TH1D h_eta("h_eta","; #eta; Yield",30,-3.5,4.0);
    TH1D h_pt("h_pt","; p_{t} [GeV];  Yield",10,95,105);
    TProfile hp_eta("hp_eta","; #eta; ",30,-4.0,4.0);

    PlotFillInstructionWithRef<TH1D, IDPVMTree> truthEta("truthEta", [](TH1D* h, IDPVMTree &t){ h->Fill(t.truth_eta());}, h_eta);
    PlotFillInstructionWithRef<TH1D, IDPVMTree> trackEta("trackEta", [](TH1D* h, IDPVMTree &t){ h->Fill(t.track_eta());}, h_eta);
 
    PlotFillInstructionWithRef<TH1D, IDPVMTree> truthPt("truthPt", [](TH1D* h, IDPVMTree &t){ h->Fill(t.truth_pt() * 0.001);}, h_pt);
    PlotFillInstructionWithRef<TH1D, IDPVMTree> trackPt("trackPt", [](TH1D* h, IDPVMTree &t){ h->Fill(t.track_pt() * 0.001);}, h_pt);

    PlotFillInstructionWithRef<TProfile, IDPVMTree> z0PullMean_eta ("z0PullMean_eta", [](TProfile* h, IDPVMTree &t){ 
        h->Fill(t.track_eta(), (t.track_z0() - t.truth_z0())/t.trackErr_z0());}, hp_eta);

    PlotFillInstructionWithRef<TProfile, IDPVMTree> z0Resolution_eta ("z0Resolution_eta", [](TProfile* h, IDPVMTree &t){ 
        h->Fill(t.track_eta(), (t.track_z0() - t.truth_z0())/t.truth_z0());}, hp_eta);

    PlotFillInstructionWithRef<TProfile, IDPVMTree> d0PullMean_eta ("d0PullMean_eta", [](TProfile* h, IDPVMTree &t){ 
        h->Fill(t.track_eta(), (t.track_d0() - t.truth_d0())/t.trackErr_d0());}, hp_eta);

    PlotFillInstructionWithRef<TProfile, IDPVMTree> d0Resolution_eta ("d0Resolution_eta", [](TProfile* h, IDPVMTree &t){ 
        h->Fill(t.track_eta(), (t.track_d0() - t.truth_d0())/t.truth_d0());}, hp_eta);

    PlotFillInstructionWithRef<TProfile, IDPVMTree> ptPullMean_eta ("ptPullMean_eta", [](TProfile* h, IDPVMTree &t){ 
        h->Fill(t.track_eta(), (t.track_pt() - t.truth_pt())/(t.track_charge() / t.trackErr_qOverPt()));}, hp_eta);

    PlotFillInstructionWithRef<TProfile, IDPVMTree> ptResolution_eta ("ptResolution_eta", [](TProfile* h, IDPVMTree &t){ 
        h->Fill(t.track_eta(), (t.track_pt() - t.truth_pt())/t.truth_pt());}, hp_eta);

    PlotContent<TH1D> plot_efficiency_eta({Plot<TH1D> (mySample, hasTruth, truthEta, "All", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kBlue)),
                                           Plot<TH1D> (mySample, passedTruth, truthEta, "isGood", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kRed))},
                                           {""}, "IDPVM/Efficiency_eta","", CanvasOptions().ratioAxisTitle("Efficiency"));

    PlotContent<TH1D> plot_fakerate_eta({Plot<TH1D> (mySample, hasTrack, trackEta, "All", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kBlue)),
                                         Plot<TH1D> (mySample, hasTrack && isFake, trackEta, "isFake", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kRed))},
                                         {""}, "IDPVM/Fakerate_eta","", CanvasOptions().ratioAxisTitle("Fake Rate"));
    
    PlotContent<TH1D> plot_yields_eta({Plot<TH1D> (mySample, passedTruth, truthEta, "Truth eta", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kRed)),
                                        Plot<TH1D> (mySample, passedTrack, trackEta, "Track eta", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kBlue))},
                                       {""}, "IDPVM/Track_vs_Truth_eta","", CanvasOptions().ratioAxisTitle("vs. Truth"));

    PlotContent<TH1D> plot_yields_pt({Plot<TH1D> (mySample, passedTruth, truthPt, "Truth pt", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kRed)),
                                      Plot<TH1D> (mySample, passedTrack, trackPt, "Track pt", "PL", PlotFormat().MarkerStyle(kFullDotLarge).Color(kBlue))},
                                      {""}, "IDPVM/Track_vs_Truth_pt","", CanvasOptions().ratioAxisTitle("vs. Truth"));

    PlotContent<TProfile> plot_z0PullMean_eta({Plot<TProfile> (mySample, passedTrack && passedTruth, z0PullMean_eta, "Test", "PL", 
                                               PlotFormat().MarkerStyle(kFullDotLarge).Color(kRed))}, {""}, "IDPVM/z0PullMean_vs_eta","", CanvasOptions().yAxisTitle("z_{0} Pull Mean"));
    
    PlotContent<TProfile> plot_z0Resolution_eta({Plot<TProfile> (mySample, passedTrack && passedTruth, z0Resolution_eta, "Test", "PL", 
                                                 PlotFormat().MarkerStyle(kFullDotLarge).Color(kRed))}, {""}, "IDPVM/z0Res_vs_eta","", CanvasOptions().yAxisTitle("z_{0} Resolution"));

    PlotContent<TProfile> plot_d0PullMean_eta({Plot<TProfile> (mySample, passedTrack && passedTruth, d0PullMean_eta, "Test", "PL", 
                                               PlotFormat().MarkerStyle(kFullDotLarge).Color(kRed))}, {""}, "IDPVM/d0PullMean_vs_eta","", CanvasOptions().yAxisTitle("d_{0} Pull Mean"));
    PlotContent<TProfile> plot_d0Resolution_eta({Plot<TProfile> (mySample, passedTrack && passedTruth, d0Resolution_eta, "Test", "PL", 
                                                 PlotFormat().MarkerStyle(kFullDotLarge).Color(kRed))}, {""}, "IDPVM/d0Res_vs_eta","", CanvasOptions().yAxisTitle("d_{0} Resolution"));

    PlotContent<TProfile> plot_ptPullMean_eta({Plot<TProfile> (mySample, passedTrack && passedTruth, ptPullMean_eta, "Test", "PL", 
                                               PlotFormat().MarkerStyle(kFullDotLarge).Color(kRed))}, {""}, "IDPVM/PtPullMean_vs_eta","", CanvasOptions().yAxisTitle("p_{T} Pull Mean"));
    PlotContent<TProfile> plot_ptResolution_eta({Plot<TProfile> (mySample, passedTrack && passedTruth, ptResolution_eta, "Test", "PL", 
                                                 PlotFormat().MarkerStyle(kFullDotLarge).Color(kRed))}, {""}, "IDPVM/PtRes_vs_eta","", CanvasOptions().yAxisTitle("p_{T} Resolution"));

    DefaultPlotting::draw1DWithRatio(plot_efficiency_eta);
    DefaultPlotting::draw1DWithRatio(plot_fakerate_eta);

    DefaultPlotting::draw1DWithRatio(plot_yields_eta);
    DefaultPlotting::draw1DWithRatio(plot_yields_pt);

    DefaultPlotting::draw1DNoRatio(plot_z0PullMean_eta);
    DefaultPlotting::draw1DNoRatio(plot_z0Resolution_eta);
    
    DefaultPlotting::draw1DNoRatio(plot_d0PullMean_eta);
    DefaultPlotting::draw1DNoRatio(plot_d0Resolution_eta);

    DefaultPlotting::draw1DNoRatio(plot_ptPullMean_eta);
    DefaultPlotting::draw1DNoRatio(plot_ptResolution_eta);
}

int main (int, char**){

    SetAtlasStyle();

    DumpResults("/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/Results/MyPhysVal.root");

    return 0;
}
