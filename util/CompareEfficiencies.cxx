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

int main (int, char**) {

    SetAtlasStyle();

    const std::string myphysval = "/Users/zschillaci/CERN/Working/IDPVMAnalysis/run/PHYSVAL.CL18.root";
    Sample<IDPVMTree> ntuple("Ntuple", myphysval, "SquirrelPlots/Ntuples/SquirrelPlots_NtuplesTruthToReco");   
    
    std::map<IDPVMDefs::variable, std::string> mapIDPVM{
        {IDPVMDefs::eta, "SquirrelPlots/Tracks/Efficiency/efficiency_vs_eta"},
        {IDPVMDefs::pt,  "SquirrelPlots/Tracks/Efficiency/efficiency_vs_pt"},
        {IDPVMDefs::d0,  "SquirrelPlots/Tracks/Efficiency/efficiency_vs_d0"},
        {IDPVMDefs::z0,  "SquirrelPlots/Tracks/Efficiency/efficiency_vs_z0"},
        {IDPVMDefs::phi, "SquirrelPlots/Tracks/Efficiency/efficiency_vs_phi"},
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
        CompareWithIDPVM(IDPVMplots.at(var.first), PlotUtils::getRatio(nums.at(var.first), dens.at(var.first), PlotUtils::efficiencyErrors), {"IDPVM Ntuple Validation", var.second});
    }

    return 0;
}
