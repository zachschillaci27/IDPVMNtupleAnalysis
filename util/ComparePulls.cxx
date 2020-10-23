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
        std::cout << "Usage: ComparePulls <optional: versus variable ('eta' or 'pt')>" << std::endl;
    }
    if (argc == 2) {
        if (IDPVMLabels::getVarName(IDPVMDefs::pt) == std::string(argv[1])) {
            versus = IDPVMDefs::pt;
        }
    }

    std::map<IDPVMDefs::variable, std::string> mapPullWidthsIDPVM{
        {IDPVMDefs::d0,    "SquirrelPlots/Tracks/Matched/Resolutions/Primary/pullwidth_vs_" + IDPVMLabels::getVarName(versus) + "_d0"},
        {IDPVMDefs::z0,    "SquirrelPlots/Tracks/Matched/Resolutions/Primary/pullwidth_vs_" + IDPVMLabels::getVarName(versus) + "_z0"},
        // {IDPVMDefs::phi,   "SquirrelPlots/Tracks/Matched/Resolutions/Primary/pullwidth_vs_" + IDPVMLabels::getVarName(versus) + "_phi"},
        // {IDPVMDefs::theta, "SquirrelPlots/Tracks/Matched/Resolutions/Primary/pullwidth_vs_" + IDPVMLabels::getVarName(versus) + "_theta"},
    };

    std::map<IDPVMDefs::variable, std::string> mapPullMeansIDPVM{
        {IDPVMDefs::d0,    "SquirrelPlots/Tracks/Matched/Resolutions/Primary/pullmean_vs_" + IDPVMLabels::getVarName(versus) + "_d0"},
        {IDPVMDefs::z0,    "SquirrelPlots/Tracks/Matched/Resolutions/Primary/pullmean_vs_" + IDPVMLabels::getVarName(versus) + "_z0"},
        // {IDPVMDefs::phi,   "SquirrelPlots/Tracks/Matched/Resolutions/Primary/pullmean_vs_" + IDPVMLabels::getVarName(versus) + "_phi"},
        // {IDPVMDefs::theta, "SquirrelPlots/Tracks/Matched/Resolutions/Primary/pullmean_vs_" + IDPVMLabels::getVarName(versus) + "_theta"},
    };

    std::map<IDPVMDefs::variable, Plot<TH1>>  IDPVMpullWidths;
    std::map<IDPVMDefs::variable, Plot<TH1>>  IDPVMpullMeans;
    std::map<IDPVMDefs::variable, Plot<TH2D>> pullHists2D;

    for (auto & var : mapPullWidthsIDPVM) {
        IDPVMpullWidths.emplace(var.first,
            LoadIDPVMHistogram<TH1F, TH1>(myphysval, mapPullWidthsIDPVM.at(var.first))); // nominal pull width plot from IDPVM
        IDPVMpullMeans.emplace(var.first,
            LoadIDPVMHistogram<TH1F, TH1>(myphysval, mapPullMeansIDPVM.at(var.first))); // nominal pull mean plot from IDPVM
    
        pullHists2D.emplace(var.first,
            Plot<TH2D>(ntuple, IDPVMSelections::forResolution(), IDPVMPopulators::getPullPopulator(var.first, versus))); // 2D pulls plot
    }
 
     for (auto & var : mapPullWidthsIDPVM) {
        pullHists2D.at(var.first).populate();
        auto pulls = GetPulls(pullHists2D.at(var.first), var.first); // extract 1D pull width and mean

        IDPVMPlotUtils::CompareWithIDPVM(IDPVMpullWidths.at(var.first), pulls.first, {"IDPVM Ntuple Validation", mapPullWidthsIDPVM.at(var.first)});
        IDPVMPlotUtils::CompareWithIDPVM(IDPVMpullMeans.at(var.first), pulls.second, {"IDPVM Ntuple Validation", mapPullMeansIDPVM.at(var.first)});
    }

    return 0;
}
