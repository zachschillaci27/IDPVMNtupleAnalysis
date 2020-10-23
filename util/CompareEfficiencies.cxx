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
#include "IDPVMNtupleAnalysis/NtupleVarReader.h"
#include "IDPVMNtupleAnalysis/IDPVMPlotUtils.h"

int main (int, char**) {

    SetAtlasStyle();

    const std::string myphysval = "/Users/zschillaci/CERN/Working/IDPVMAnalysis/run/M_output.root";
    Sample<IDPVMTree> ntuple("Ntuple", myphysval, "SquirrelPlots/Ntuples/SquirrelPlots_NtuplesTruthToReco");   
    
    std::map<IDPVMDefs::variable, std::string> mapIDPVM{
        {IDPVMDefs::eta,   "SquirrelPlots/Tracks/Efficiency/efficiency_vs_eta"},
        {IDPVMDefs::pt,    "SquirrelPlots/Tracks/Efficiency/efficiency_vs_pt"},
        {IDPVMDefs::d0,    "SquirrelPlots/Tracks/Efficiency/efficiency_vs_d0"},
        {IDPVMDefs::z0,    "SquirrelPlots/Tracks/Efficiency/efficiency_vs_z0"},
        {IDPVMDefs::phi,   "SquirrelPlots/Tracks/Efficiency/efficiency_vs_phi"},
        // {IDPVMDefs::theta, "SquirrelPlots/Tracks/Efficiency/efficiency_vs_theta"},
    };
    
    std::map<IDPVMDefs::variable, Plot<TH1>>  IDPVMplots;
    std::map<IDPVMDefs::variable, Plot<TH1D>> nums;
    std::map<IDPVMDefs::variable, Plot<TH1D>> dens;

    for (auto & var : mapIDPVM) {
        IDPVMplots.emplace(var.first,
            LoadIDPVMEfficiency(myphysval, var.second)); // nominal plot from IDPVM

        auto varReader = NtupleVarReaderProvider::generateVarReader(var.first, IDPVMDefs::truth); // truth variable reader
        auto htemplate = IDPVMTemplates::getEfficiencyHistTemplate(var.first); // template histogram

        PlotFillInstructionWithRef<TH1D, IDPVMTree> filler(IDPVMLabels::getVarName(var.first), // plot filler
            [=](TH1D* h, IDPVMTree &t){ h->Fill(varReader(t)); },
            htemplate
        );

        nums.emplace(var.first,
            Plot<TH1D>(ntuple, IDPVMSelections::forEfficiencyNum(), filler)); // numerator plot
        dens.emplace(var.first,
            Plot<TH1D>(ntuple, IDPVMSelections::forEfficiencyDen(), filler)); // denominator plot
    }

    for (auto & var : mapIDPVM) {
        nums.at(var.first).populate();
        dens.at(var.first).populate();
        IDPVMPlotUtils::CompareWithIDPVM(IDPVMplots.at(var.first), PlotUtils::getRatio(nums.at(var.first), dens.at(var.first), PlotUtils::efficiencyErrors), {"IDPVM Ntuple Validation", var.second});
    }

    return 0;
}
