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
    
    std::map<IDPVMDefs::variable, std::string> mapEfficiency{
        {IDPVMDefs::eta, "SquirrelPlots/Tracks/Efficiency/efficiency_vs_eta"},
        {IDPVMDefs::pt,  "SquirrelPlots/Tracks/Efficiency/efficiency_vs_pt"},
        {IDPVMDefs::d0,  "SquirrelPlots/Tracks/Efficiency/efficiency_vs_d0"},
        {IDPVMDefs::z0,  "SquirrelPlots/Tracks/Efficiency/efficiency_vs_z0"},
    };
    
    for (auto & var : mapEfficiency) {
        auto nominal = LoadIDPVMEfficiency(myphysval, var.second); // nominal plot from IDPVM

        auto varReader = NtupleVarReaderProvider::generateVarReader(var.first, IDPVMDefs::truth); // truth variable reader
        auto htemplate = IDPVMTemplates::getEfficiencyHistTemplate(var.first); // hist template

        PlotFillInstructionWithRef<TH1D, IDPVMTree> filler(IDPVMLabels::getVarName(var.first), // plot filler
            [&varReader](TH1D* h, IDPVMTree &t){ h->Fill(varReader(t)); },
            htemplate
        );

        Plot<TH1D> num(ntuple, IDPVMSelections::forEfficiencyNum(), filler); // numerator plot
        Plot<TH1D> den(ntuple, IDPVMSelections::forEfficiencyDen(), filler); // denominator plot

        num.populate();
        den.populate();
    
        CompareWithIDPVM(nominal, PlotUtils::getRatio(num, den, PlotUtils::efficiencyErrors), {"IDPVM Ntuple Validation", "t#bar{t}"});
    }

    return 0;
}
