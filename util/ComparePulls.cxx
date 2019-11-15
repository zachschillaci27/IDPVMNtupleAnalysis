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

        PlotContent<TH1> theContent({thePair.first, thePair.second}, labels, thePair.first.getName(), "",
                                     CanvasOptions().labelLumiTag("HL-LHC").labelSqrtsTag("14 TeV").yAxisTitle(thePair.second->GetYaxis()->GetTitle()).ratioAxisTitle("Ntuple/IDPVM"));
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

    Plot<TH2D> ptPullTH2D(ntuple, selResolutions, IDPVMPopulators::getPullPopulator(IDPVMDefs::pt, versus));
    Plot<TH2D> d0PullTH2D(ntuple, selResolutions, IDPVMPopulators::getPullPopulator(IDPVMDefs::d0, versus));
    Plot<TH2D> z0PullTH2D(ntuple, selResolutions, IDPVMPopulators::getPullPopulator(IDPVMDefs::z0, versus));
    Plot<TH2D> z0sinPullTH2D(ntuple, selResolutions, IDPVMPopulators::getPullPopulator(IDPVMDefs::z0sin, versus));
    Plot<TH2D> phiPullTH2D(ntuple, selResolutions, IDPVMPopulators::getPullPopulator(IDPVMDefs::phi, versus));
    Plot<TH2D> thetaPullTH2D(ntuple, selResolutions, IDPVMPopulators::getPullPopulator(IDPVMDefs::theta, versus));
    Plot<TH2D> qOverPtPullTH2D(ntuple, selResolutions, IDPVMPopulators::getPullPopulator(IDPVMDefs::qOverPt, versus));
    Plot<TH2D> qOverPPullTH2D(ntuple, selResolutions, IDPVMPopulators::getPullPopulator(IDPVMDefs::qOverP, versus));

    ptPullTH2D.populate();
    d0PullTH2D.populate();
    z0PullTH2D.populate();
    z0sinPullTH2D.populate();
    phiPullTH2D.populate();
    thetaPullTH2D.populate();
    qOverPtPullTH2D.populate();
    qOverPPullTH2D.populate();

    std::pair<Plot<TH1>, Plot<TH1>> ptPulls = GetPulls(ptPullTH2D, IDPVMDefs::pt);
    std::pair<Plot<TH1>, Plot<TH1>> d0Pulls = GetPulls(d0PullTH2D, IDPVMDefs::d0);
    std::pair<Plot<TH1>, Plot<TH1>> z0Pulls = GetPulls(z0PullTH2D, IDPVMDefs::z0);
    std::pair<Plot<TH1>, Plot<TH1>> z0sinPulls = GetPulls(z0PullTH2D, IDPVMDefs::z0sin);
    std::pair<Plot<TH1>, Plot<TH1>> phiPulls = GetPulls(phiPullTH2D, IDPVMDefs::phi);
    std::pair<Plot<TH1>, Plot<TH1>> thetaPulls = GetPulls(thetaPullTH2D, IDPVMDefs::theta);
    std::pair<Plot<TH1>, Plot<TH1>> qOverPtPulls = GetPulls(qOverPtPullTH2D, IDPVMDefs::qOverPt);
    std::pair<Plot<TH1>, Plot<TH1>> qOverPPulls = GetPulls(qOverPtPullTH2D, IDPVMDefs::qOverP);

    Plot<TH1> ptPullWidthIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/ptpullresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> d0PullWidthIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0pullresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> z0PullWidthIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0pullresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> z0sinPullWidthIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0sinpullresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> phiPullWidthIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/phipullresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> thetaPullWidthIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/thetapullresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> qOverPtPullWidthIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverptpullresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> qOverPPullWidthIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverppullresolutionRMS_vs_" + IDPVMLabels::getVarName(versus));

    Plot<TH1> ptPullMeanIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/ptpullmeanRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> d0PullMeanIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0pullmeanRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> z0PullMeanIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0pullmeanRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> z0sinPullMeanIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0sinpullmeanRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> phiPullMeanIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/phipullmeanRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> thetaPullMeanIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/thetapullmeanRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> qOverPtPullMeanIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverptpullmeanRMS_vs_" + IDPVMLabels::getVarName(versus));
    Plot<TH1> qOverPPullMeanIDPVM = LoadIDPVMHistogram<TH1>(myphysval, "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverppullmeanRMS_vs_" + IDPVMLabels::getVarName(versus));

    std::vector<std::pair<Plot<TH1>, Plot<TH1>>> PullWidths = {
        std::make_pair(ptPullWidthIDPVM, ptPulls.first),
        std::make_pair(d0PullWidthIDPVM, d0Pulls.first),
        std::make_pair(z0PullWidthIDPVM, z0Pulls.first),
        std::make_pair(z0sinPullWidthIDPVM, z0sinPulls.first),
        std::make_pair(phiPullWidthIDPVM, phiPulls.first),
        std::make_pair(thetaPullWidthIDPVM, thetaPulls.first),  
        std::make_pair(qOverPtPullWidthIDPVM, qOverPtPulls.first),  
        std::make_pair(qOverPPullWidthIDPVM, qOverPPulls.first),  
    };

    std::vector<std::pair<Plot<TH1>, Plot<TH1>>> PullMeans = {
        std::make_pair(ptPullMeanIDPVM, ptPulls.second),
        std::make_pair(d0PullMeanIDPVM, d0Pulls.second),
        std::make_pair(z0PullMeanIDPVM, z0Pulls.second),
        std::make_pair(z0sinPullMeanIDPVM, z0sinPulls.second),
        std::make_pair(phiPullMeanIDPVM, phiPulls.second),
        std::make_pair(thetaPullMeanIDPVM, thetaPulls.second),  
        std::make_pair(qOverPtPullMeanIDPVM, qOverPtPulls.second),  
        std::make_pair(qOverPPullMeanIDPVM, qOverPPulls.second),  
    };

    CompareWithIDPVM(PullWidths, {"IDPVM Ntuple Validation", "t#bar{t}"});
    CompareWithIDPVM(PullMeans, {"IDPVM Ntuple Validation", "t#bar{t}"});

    return 0;
}
