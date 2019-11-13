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

void CompareIDPVM(std::vector<std::pair<Plot<TH1>, Plot<TH1>>> thePlotPairs, const std::vector<std::string> & labels) {    
    for (auto & thePair : thePlotPairs) {
        thePair.first.setLegendTitle("IDPVM - Reference");
        thePair.first.setLegendOption("PL");
        thePair.first.setPlotFormat(PlotFormat().MarkerStyle(kFullDotLarge).MarkerColor(kRed + 1).LineColor(kRed + 1));
        
        thePair.second.setLegendTitle("IDPVM - Alternative");
        thePair.second.setLegendOption("PL");
        thePair.second.setPlotFormat(PlotFormat().MarkerStyle(kOpenCircle).MarkerColor(kBlue + 1).LineColor(kBlue + 1));

        const std::string fname = thePair.first.getName() + "-Binning";
        const bool logY = (thePair.first.getName().find("z0") == std::string::npos) ? false : true;

        PlotContent<TH1> theContent({thePair.first, thePair.second}, labels, fname, "",
                                     CanvasOptions().labelLumiTag("HL-LHC").labelSqrtsTag("14 TeV").logY(logY).yAxisTitle(thePair.second->GetYaxis()->GetTitle()).ratioAxisTitle("Alt/Ref"));
        DefaultPlotting::draw1DWithRatio(theContent);
    }
}

int main (int, char**) {

    SetAtlasStyle();

    const std::string reference = "/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/sglmu100/reference/MyPhysVal.root";
    const std::string alternative = "/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/sglmu100/alternative/MyPhysVal.root";

    Plot<TH1> d0ResIDPVM = LoadIDPVMHistogram<TH1>(reference, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0resolutionRMS_vs_eta");
    Plot<TH1> z0ResIDPVM = LoadIDPVMHistogram<TH1>(reference, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0resolutionRMS_vs_eta");
    Plot<TH1> phiResIDPVM = LoadIDPVMHistogram<TH1>(reference, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/phiresolutionRMS_vs_eta");
    Plot<TH1> thetaResIDPVM = LoadIDPVMHistogram<TH1>(reference, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/thetaresolutionRMS_vs_eta");
    Plot<TH1> qOverPtResIDPVM = LoadIDPVMHistogram<TH1>(reference, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverptresolutionRMS_vs_eta");

    Plot<TH1> d0ResAlternativeIDPVM = LoadIDPVMHistogram<TH1>(alternative, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0resolutionRMS_vs_eta");
    Plot<TH1> z0ResAlternativeIDPVM = LoadIDPVMHistogram<TH1>(alternative, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0resolutionRMS_vs_eta");
    Plot<TH1> phiResAlternativeIDPVM = LoadIDPVMHistogram<TH1>(alternative, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/phiresolutionRMS_vs_eta");
    Plot<TH1> thetaResAlternativeIDPVM = LoadIDPVMHistogram<TH1>(alternative, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/thetaresolutionRMS_vs_eta");
    Plot<TH1> qOverPtResAlternativeIDPVM = LoadIDPVMHistogram<TH1>(alternative, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverptresolutionRMS_vs_eta");

    std::vector<std::pair<Plot<TH1>, Plot<TH1>>> Resolutions = {
        std::make_pair(d0ResIDPVM, d0ResAlternativeIDPVM),
        std::make_pair(z0ResIDPVM, z0ResAlternativeIDPVM),
        std::make_pair(phiResIDPVM, phiResAlternativeIDPVM),
        std::make_pair(thetaResIDPVM, thetaResAlternativeIDPVM),  
        std::make_pair(qOverPtResIDPVM, qOverPtResAlternativeIDPVM),  
    }; 

    CompareIDPVM(Resolutions, {"IDPVM Binning Migrations"});

    return 0;
}
