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
#include "IDPVMNtupleAnalysis/IDPVMPopulators.h"
#include "IDPVMNtupleAnalysis/IDPVMUtilities.h"
#include "IDPVMNtupleAnalysis/IDPVMSelections.h"
#include "IDPVMNtupleAnalysis/ResolutionHelper.h"

void CompareWithIDPVM(std::vector<std::pair<Plot<TH1>, Plot<TH1>>> thePlotPairs, const std::vector<std::string> & labels) { 
    for (auto & thePair : thePlotPairs) {
        thePair.first.setLegendTitle("IDPVM");
        thePair.first.setLegendOption("PL");
        thePair.first.setPlotFormat(PlotFormat().MarkerStyle(kFullDotLarge).MarkerColor(kRed + 1).LineColor(kRed + 1));
        
        thePair.second.setLegendTitle("Ntuple");
        thePair.second.setLegendOption("PL");
        thePair.second.setPlotFormat(PlotFormat().MarkerStyle(kOpenCircle).MarkerColor(kBlue + 1).LineColor(kBlue + 1));

        const bool logY = (thePair.first.getName().find("z0") == std::string::npos) ? false : true;

        PlotContent<TH1> theContent({thePair.first, thePair.second}, labels, thePair.first.getName(), "",
                                     CanvasOptions().labelLumiTag("HL-LHC").labelSqrtsTag("14 TeV").logY(logY).yAxisTitle(thePair.second->GetYaxis()->GetTitle()).ratioAxisTitle("Ntuple/IDPVM"));
        DefaultPlotting::draw1DWithRatio(theContent);
    }
}

int main (int argc, char** argv) {
    SetAtlasStyle();

    const std::string myphysval = "/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/ttbar/alternative/MyPhysVal.root";
    Sample<IDPVMTree> ntuple("", myphysval, "IDPerformanceMon/Ntuples/IDPerformanceMon_NtuplesTruthToReco");   

    Selection<IDPVMTree> selResolutions = IDPVMSelections::forResolution();

    IDPVMDefs::variable versus = IDPVMDefs::eta;
    if (argc < 2) {
        std::cout << "Usage: CompareResolutions <optional: versus variable ('eta' or 'pt')>" << std::endl;
    }
    if (argc == 2) {
        if (IDPVMLabels::getVarName(IDPVMDefs::pt) == std::string(argv[1])) {
            versus = IDPVMDefs::pt;
        }
    }

    Plot<TH2D> ptResTH2D(ntuple, selResolutions, IDPVMPopulators::getResolutionPopulator(IDPVMDefs::pt, versus));
    Plot<TH2D> d0ResTH2D(ntuple, selResolutions, IDPVMPopulators::getResolutionPopulator(IDPVMDefs::d0, versus)); 
    Plot<TH2D> z0ResTH2D(ntuple, selResolutions, IDPVMPopulators::getResolutionPopulator(IDPVMDefs::z0, versus)); 
    Plot<TH2D> z0sinResTH2D(ntuple, selResolutions, IDPVMPopulators::getResolutionPopulator(IDPVMDefs::z0sin, versus));
    Plot<TH2D> phiResTH2D(ntuple, selResolutions, IDPVMPopulators::getResolutionPopulator(IDPVMDefs::phi, versus));
    Plot<TH2D> thetaResTH2D(ntuple, selResolutions, IDPVMPopulators::getResolutionPopulator(IDPVMDefs::theta, versus));
    Plot<TH2D> qOverPtResTH2D(ntuple, selResolutions, IDPVMPopulators::getResolutionPopulator(IDPVMDefs::qOverPt, versus));
    Plot<TH2D> qOverPResTH2D(ntuple, selResolutions, IDPVMPopulators::getResolutionPopulator(IDPVMDefs::qOverP, versus));

    ptResTH2D.populate();
    d0ResTH2D.populate();
    z0ResTH2D.populate();
    z0sinResTH2D.populate();
    phiResTH2D.populate();
    thetaResTH2D.populate();
    qOverPtResTH2D.populate();
    qOverPResTH2D.populate();

    Plot<TH1> ptRes = GetResolution(ptResTH2D, IDPVMDefs::pt);
    Plot<TH1> d0Res = GetResolution(d0ResTH2D, IDPVMDefs::d0);
    Plot<TH1> z0Res = GetResolution(z0ResTH2D, IDPVMDefs::z0);
    Plot<TH1> z0sinRes = GetResolution(z0sinResTH2D, IDPVMDefs::z0sin);
    Plot<TH1> phiRes = GetResolution(phiResTH2D, IDPVMDefs::phi);
    Plot<TH1> thetaRes = GetResolution(thetaResTH2D, IDPVMDefs::theta);
    Plot<TH1> qOverPtRes = GetResolution(qOverPtResTH2D, IDPVMDefs::qOverPt);
    Plot<TH1> qOverPRes = GetResolution(qOverPResTH2D, IDPVMDefs::qOverP);

    Plot<TH1> ptResIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/ptresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> d0ResIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0resolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> z0ResIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0resolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> z0sinResIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0sinresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> phiResIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/phiresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> thetaResIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/thetaresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> qOverPtResIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverptresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> qOverPResIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverpresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));

    std::vector<std::pair<Plot<TH1>, Plot<TH1>>> Resolutions = {
        std::make_pair(ptResIDPVM, ptRes),
        std::make_pair(d0ResIDPVM, d0Res),
        std::make_pair(z0ResIDPVM, z0Res),
        std::make_pair(z0sinResIDPVM, z0sinRes),
        std::make_pair(phiResIDPVM, phiRes),
        std::make_pair(thetaResIDPVM, thetaRes),  
        std::make_pair(qOverPtResIDPVM, qOverPtRes),  
        std::make_pair(qOverPResIDPVM, qOverPRes),
    }; 

    CompareWithIDPVM(Resolutions, {"IDPVM Ntuple Validation", "t#bar{t}"});

    return 0;
}
