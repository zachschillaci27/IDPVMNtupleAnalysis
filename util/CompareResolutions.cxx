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
    can->SetLogy();

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

    TH2D hRes_d0_vs_eta = IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::d0, IDPVMDefs::eta);
    TH2D hRes_z0_vs_eta = IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::z0, IDPVMDefs::eta);
    TH2D hRes_phi_vs_eta = IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::phi, IDPVMDefs::eta);
    TH2D hRes_theta_vs_eta = IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::theta, IDPVMDefs::eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> d0Resolution_eta ("d0Resolution_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_d0() - t.truth_d0()));}, hRes_d0_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> z0Resolution_eta ("z0Resolution_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_z0() - t.truth_z0()));}, hRes_z0_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> phiResolution_eta ("phiResolution_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_phi() - t.truth_phi()));}, hRes_phi_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> thetaResolution_eta ("thetaResolution_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_theta() - t.truth_theta()));}, hRes_theta_vs_eta);

    Plot<TH2D> d0ResTH2D(myPhysVal, matchedPrimaryTracks, d0Resolution_eta);
    Plot<TH2D> z0ResTH2D(myPhysVal, matchedPrimaryTracks, z0Resolution_eta);
    Plot<TH2D> phiResTH2D(myPhysVal, matchedPrimaryTracks, phiResolution_eta);
    Plot<TH2D> thetaResTH2D(myPhysVal, matchedPrimaryTracks, thetaResolution_eta);

    d0ResTH2D.populate();
    z0ResTH2D.populate();
    phiResTH2D.populate();
    thetaResTH2D.populate();

    Plot<TH1D> d0Res = GetResolution(d0ResTH2D, IDPVMDefs::d0);
    Plot<TH1D> z0Res = GetResolution(z0ResTH2D, IDPVMDefs::z0);
    Plot<TH1D> phiRes = GetResolution(phiResTH2D, IDPVMDefs::phi);
    Plot<TH1D> thetaRes = GetResolution(thetaResTH2D, IDPVMDefs::theta);

    TH1* d0ResIDPVM = ReadHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0resolutionRMS_vs_eta");
    TH1* z0ResIDPVM = ReadHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0resolutionRMS_vs_eta");
    TH1* phiResIDPVM = ReadHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/phiresolutionRMS_vs_eta");
    TH1* thetaResIDPVM = ReadHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/thetaresolutionRMS_vs_eta");

    CompareWithIDPVM<Plot<TH1D>, TH1>(d0Res, d0ResIDPVM, "d0Resolution");
    CompareWithIDPVM<Plot<TH1D>, TH1>(z0Res, z0ResIDPVM, "z0Resolution");
    CompareWithIDPVM<Plot<TH1D>, TH1>(phiRes, phiResIDPVM, "phiResolution");
    CompareWithIDPVM<Plot<TH1D>, TH1>(thetaRes, thetaResIDPVM, "thetaResolution");

    return 0;
}
