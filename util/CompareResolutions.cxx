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

        const bool logY = (thePair.first.getName().find("z0") == std::string::npos) ? false : true;

        PlotContent<TH1> theContent({thePair.first, thePair.second}, {"ITk Step 3.0", "IDPVM Ntuple Validation"}, thePair.first.getName(), "",
                                     CanvasOptions().logY(logY).yAxisTitle(thePair.second->GetYaxis()->GetTitle()).ratioAxisTitle("Ntuple/IDPVM"));
        DefaultPlotting::draw1DWithRatio(theContent);
    }
}

int main (int, char**) {

    SetAtlasStyle();

    const std::string myphysval = "/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/sglmu100/alternative/MyPhysVal.root";
    Sample<IDPVMTree> ntuple("", myphysval, "IDPerformanceMon/Ntuples/IDPerformanceMon_NtuplesTruthToReco");   

    Selection<IDPVMTree> selResolutions = IDPVMSelections::forResolution();

    TH2D hRes_d0_vs_eta = IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::d0, IDPVMDefs::eta);
    TH2D hRes_z0_vs_eta = IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::z0, IDPVMDefs::eta);
    TH2D hRes_phi_vs_eta = IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::phi, IDPVMDefs::eta);
    TH2D hRes_theta_vs_eta = IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::theta, IDPVMDefs::eta);
    TH2D hRes_qOverPt_vs_eta = IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::qOverPt, IDPVMDefs::eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> d0Resolution_eta ("d0Resolution_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_d0() - t.truth_d0()));}, hRes_d0_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> z0Resolution_eta ("z0Resolution_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_z0() - t.truth_z0()));}, hRes_z0_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> phiResolution_eta ("phiResolution_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_phi() - t.truth_phi()));}, hRes_phi_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> thetaResolution_eta ("thetaResolution_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_theta() - t.truth_theta()));}, hRes_theta_vs_eta);

    PlotFillInstructionWithRef<TH2D, IDPVMTree> qOverPtResolution_eta ("qOverPtResolution_eta", [](TH2D* h, IDPVMTree &t){ 
        h->Fill(t.truth_eta(), (t.track_qOverPt() - t.truth_qOverPt())/t.truth_qOverPt());}, hRes_qOverPt_vs_eta);

    Plot<TH2D> d0ResTH2D(ntuple, selResolutions, d0Resolution_eta);
    Plot<TH2D> z0ResTH2D(ntuple, selResolutions, z0Resolution_eta);
    Plot<TH2D> phiResTH2D(ntuple, selResolutions, phiResolution_eta);
    Plot<TH2D> thetaResTH2D(ntuple, selResolutions, thetaResolution_eta);
    Plot<TH2D> qOverPtResTH2D(ntuple, selResolutions, qOverPtResolution_eta);

    d0ResTH2D.populate();
    z0ResTH2D.populate();
    phiResTH2D.populate();
    thetaResTH2D.populate();
    qOverPtResTH2D.populate();

    Plot<TH1> d0Res = GetResolution(d0ResTH2D, IDPVMDefs::d0);
    Plot<TH1> z0Res = GetResolution(z0ResTH2D, IDPVMDefs::z0);
    Plot<TH1> phiRes = GetResolution(phiResTH2D, IDPVMDefs::phi);
    Plot<TH1> thetaRes = GetResolution(thetaResTH2D, IDPVMDefs::theta);
    Plot<TH1> qOverPtRes = GetResolution(qOverPtResTH2D, IDPVMDefs::qOverPt);

    Plot<TH1> d0ResIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0resolutionRMS_vs_eta");
    Plot<TH1> z0ResIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0resolutionRMS_vs_eta");
    Plot<TH1> phiResIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/phiresolutionRMS_vs_eta");
    Plot<TH1> thetaResIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/thetaresolutionRMS_vs_eta");
    Plot<TH1> qOverPtResIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverptresolutionRMS_vs_eta");

    std::vector<std::pair<Plot<TH1>, Plot<TH1>>> Resolutions = {
        std::make_pair(d0ResIDPVM, d0Res),
        std::make_pair(z0ResIDPVM, z0Res),
        std::make_pair(phiResIDPVM, phiRes),
        std::make_pair(thetaResIDPVM, thetaRes),  
        std::make_pair(qOverPtResIDPVM, qOverPtRes),  
    }; 

    CompareWithIDPVM(Resolutions);

    return 0;
}
