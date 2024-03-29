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
#include "IDPVMNtupleAnalysis/IDPVMPlotUtils.h"

int main (int argc, char** argv) {
    SetAtlasStyle();

    const std::string myphysval = "/Users/zschillaci/CERN/Working/IDPVMAnalysis/run/M_output.root";
    Sample<IDPVMTree> ntuple("Ntuple", myphysval, "SquirrelPlots/Ntuples/SquirrelPlots_NtuplesTruthToReco");   

    IDPVMDefs::variable versus = IDPVMDefs::eta;
    if (argc < 2) {
        std::cout << "Usage: CompareResolutions <optional: versus variable ('eta' or 'pt')>" << std::endl;
    }
    if (argc == 2) {
        if (IDPVMLabels::getVarName(IDPVMDefs::pt) == std::string(argv[1])) {
            versus = IDPVMDefs::pt;
        }
    }

    std::map<IDPVMDefs::variable, std::string> mapIDPVM{
        {IDPVMDefs::d0,    "SquirrelPlots/Tracks/Matched/Resolutions/Primary/resolution_vs_" + IDPVMLabels::getVarName(versus) + "_d0"},
        {IDPVMDefs::z0,    "SquirrelPlots/Tracks/Matched/Resolutions/Primary/resolution_vs_" + IDPVMLabels::getVarName(versus) + "_z0"},
        // {IDPVMDefs::phi,   "SquirrelPlots/Tracks/Matched/Resolutions/Primary/resolution_vs_" + IDPVMLabels::getVarName(versus) + "_phi"},
        // {IDPVMDefs::theta, "SquirrelPlots/Tracks/Matched/Resolutions/Primary/resolution_vs_" + IDPVMLabels::getVarName(versus) + "_theta"},
    };

    std::map<IDPVMDefs::variable, Plot<TH1>>  IDPVMplots;
    std::map<IDPVMDefs::variable, Plot<TH2D>> resHists2D;

    for (auto & var : mapIDPVM) {
        IDPVMplots.emplace(var.first, 
            LoadIDPVMHistogram<TH1F, TH1>(myphysval, var.second)); // nominal plot from IDPVM

        resHists2D.emplace(var.first,
            Plot<TH2D>(ntuple, IDPVMSelections::forResolution(), IDPVMPopulators::getResolutionPopulator(var.first, versus))); // 2D resolution plot
    }

    for (auto & var : mapIDPVM) {
        resHists2D.at(var.first).populate();
        auto resHist = GetResolution(resHists2D.at(var.first), var.first); // extract 1D resolution plot
        
        IDPVMPlotUtils::CompareWithIDPVM(IDPVMplots.at(var.first), resHist, {"IDPVM Ntuple Validation", var.second});
    }
    
    return 0;
}