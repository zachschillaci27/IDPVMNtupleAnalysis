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
    
    std::map<IDPVMDefs::variable, std::string> mapFakeIDPVM{
        {IDPVMDefs::eta,   "SquirrelPlots/Tracks/FakeRate/fakerate_vs_eta"},
        {IDPVMDefs::pt,    "SquirrelPlots/Tracks/FakeRate/fakerate_vs_pt"},
        {IDPVMDefs::d0,    "SquirrelPlots/Tracks/FakeRate/fakerate_vs_d0"},
        {IDPVMDefs::z0,    "SquirrelPlots/Tracks/FakeRate/fakerate_vs_z0"},
        {IDPVMDefs::phi,   "SquirrelPlots/Tracks/FakeRate/fakerate_vs_phi"},
    };

    std::map<IDPVMDefs::variable, std::string> mapUnlinkedIDPVM{
        {IDPVMDefs::eta,   "SquirrelPlots/Tracks/Unlinked/FakeRate/fakerate_vs_eta"},
        {IDPVMDefs::pt,    "SquirrelPlots/Tracks/Unlinked/FakeRate/fakerate_vs_pt"},
        {IDPVMDefs::d0,    "SquirrelPlots/Tracks/Unlinked/FakeRate/fakerate_vs_d0"},
        {IDPVMDefs::z0,    "SquirrelPlots/Tracks/Unlinked/FakeRate/fakerate_vs_z0"},
        {IDPVMDefs::phi,   "SquirrelPlots/Tracks/Unlinked/FakeRate/fakerate_vs_phi"},
    };

    std::map<IDPVMDefs::variable, Plot<TH1>>  IDPVMfakePlots;
    std::map<IDPVMDefs::variable, Plot<TH1D>> fakeNums;
    std::map<IDPVMDefs::variable, Plot<TH1D>> fakeDens;

    std::map<IDPVMDefs::variable, Plot<TH1>>  IDPVMunlinkedPlots;
    std::map<IDPVMDefs::variable, Plot<TH1D>> unlinkedNums;
    std::map<IDPVMDefs::variable, Plot<TH1D>> unlinkedDens;

    for (auto & var : mapFakeIDPVM) {
        IDPVMfakePlots.emplace(var.first,
            LoadIDPVMEfficiency(myphysval, mapFakeIDPVM.at(var.first))); // nominal fake plot from IDPVM

        IDPVMunlinkedPlots.emplace(var.first,
            LoadIDPVMEfficiency(myphysval, mapUnlinkedIDPVM.at(var.first))); // nominal unlinked plot from IDPVM

        auto varReader = NtupleVarReaderProvider::generateVarReader(var.first, IDPVMDefs::track); // track variable reader
        auto htemplate = IDPVMTemplates::getFakeRateHistTemplate(var.first); // template histogram

        PlotFillInstructionWithRef<TH1D, IDPVMTree> filler(IDPVMLabels::getVarName(var.first), // plot filler
            [=](TH1D* h, IDPVMTree &t){ h->Fill(varReader(t)); },
            htemplate
        );

        fakeNums.emplace(var.first,
            Plot<TH1D>(ntuple, IDPVMSelections::forFakeRateNum(), filler)); // fake numerator plot
        fakeDens.emplace(var.first,
            Plot<TH1D>(ntuple, IDPVMSelections::forFakeRateDen(), filler)); // fake denominator plot

        unlinkedNums.emplace(var.first,
            Plot<TH1D>(ntuple, IDPVMSelections::forUnlinkedRateNum(), filler)); // unlinked numerator plot
        unlinkedDens.emplace(var.first,
            Plot<TH1D>(ntuple, IDPVMSelections::forUnlinkedRateDen(), filler)); // unlinked denominator plot
    }

    for (auto & var : mapFakeIDPVM) {
        fakeNums.at(var.first).populate();
        fakeDens.at(var.first).populate();
        unlinkedNums.at(var.first).populate();
        unlinkedDens.at(var.first).populate();    
        IDPVMPlotUtils::CompareWithIDPVM(IDPVMfakePlots.at(var.first), PlotUtils::getRatio(fakeNums.at(var.first), fakeDens.at(var.first), PlotUtils::efficiencyErrors), {"IDPVM Ntuple Validation", mapFakeIDPVM.at(var.first)});
        IDPVMPlotUtils::CompareWithIDPVM(IDPVMunlinkedPlots.at(var.first), PlotUtils::getRatio(unlinkedNums.at(var.first), unlinkedDens.at(var.first), PlotUtils::efficiencyErrors), {"IDPVM Ntuple Validation", mapUnlinkedIDPVM.at(var.first)}, "unlinked");    
    }

    return 0;
}