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

void CompareWithIDPVM(Plot<TH1> nominal, Plot<TH1> alternative, const std::vector<std::string> & labels) { 
    nominal.setLegendTitle("IDPVM");
    nominal.setLegendOption("PL");
    nominal.setPlotFormat(PlotFormat().MarkerStyle(kFullDotLarge).MarkerColor(kRed + 1).LineColor(kRed + 1));
    
    alternative.setLegendTitle("Ntuple");
    alternative.setLegendOption("PL");
    alternative.setPlotFormat(PlotFormat().MarkerStyle(kOpenCircle).MarkerColor(kBlue + 1).LineColor(kBlue + 1));

    PlotContent<TH1> theContent({nominal, alternative}, labels, nominal.getName(), "",
                                    CanvasOptions().yAxisTitle(alternative->GetYaxis()->GetTitle()).ratioAxisTitle("Ntuple/IDPVM"));
    DefaultPlotting::draw1DWithRatio(theContent);
}

int main (int argc, char** argv) {
    SetAtlasStyle();

    const std::string myphysval = "/Users/zschillaci/CERN/Working/IDPVMAnalysis/run/PHYSVAL.CL18.root";
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
        {IDPVMDefs::phi,   "SquirrelPlots/Tracks/Matched/Resolutions/Primary/resolution_vs_" + IDPVMLabels::getVarName(versus) + "_phi"},
        {IDPVMDefs::theta, "SquirrelPlots/Tracks/Matched/Resolutions/Primary/resolution_vs_" + IDPVMLabels::getVarName(versus) + "_theta"},
    };

    for (auto & var : mapIDPVM) {
        auto nominal = LoadIDPVMHistogram<TH1F, TH1>(myphysval, var.second); // nominal plot from IDPVM

        Plot<TH2D> resHist2D(ntuple, IDPVMSelections::forResolution(), IDPVMPopulators::getResolutionPopulator(var.first, versus)); // 2D resolution plot
        resHist2D.populate();
 
        auto resHist = GetResolution(resHist2D, var.first); // extract 1D resolution plot

        CompareWithIDPVM(nominal, resHist, {"IDPVM Ntuple Validation", "t#bar{t}"});
    }
    
    return 0;
}