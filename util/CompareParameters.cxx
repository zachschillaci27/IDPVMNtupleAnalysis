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
    
    std::map<IDPVMDefs::variable, std::string> mapIDPVMReco{
        {IDPVMDefs::eta,    "SquirrelPlots/Tracks/Selected/Parameters/reco_eta"},
        {IDPVMDefs::pt,     "SquirrelPlots/Tracks/Selected/Parameters/reco_pt"},
        {IDPVMDefs::d0,     "SquirrelPlots/Tracks/Selected/Parameters/reco_d0"},
        {IDPVMDefs::z0,     "SquirrelPlots/Tracks/Selected/Parameters/reco_z0"},
        {IDPVMDefs::phi,    "SquirrelPlots/Tracks/Selected/Parameters/reco_phi"},
        {IDPVMDefs::theta,  "SquirrelPlots/Tracks/Selected/Parameters/reco_theta"},
        {IDPVMDefs::z0sin,  "SquirrelPlots/Tracks/Selected/Parameters/reco_z0sin"},
        {IDPVMDefs::qOverP, "SquirrelPlots/Tracks/Selected/Parameters/reco_qoverp"},
    };
    
    std::map<IDPVMDefs::variable, std::string> mapIDPVMTruth{
        {IDPVMDefs::eta,    "SquirrelPlots/Tracks/Selected/Parameters/truth_eta"},
        {IDPVMDefs::pt,     "SquirrelPlots/Tracks/Selected/Parameters/truth_pt"},
        {IDPVMDefs::d0,     "SquirrelPlots/Tracks/Selected/Parameters/truth_d0"},
        {IDPVMDefs::z0,     "SquirrelPlots/Tracks/Selected/Parameters/truth_z0"},
        {IDPVMDefs::phi,    "SquirrelPlots/Tracks/Selected/Parameters/truth_phi"},
        {IDPVMDefs::theta,  "SquirrelPlots/Tracks/Selected/Parameters/truth_theta"},
        {IDPVMDefs::z0sin,  "SquirrelPlots/Tracks/Selected/Parameters/truth_z0sin"},
        {IDPVMDefs::qOverP, "SquirrelPlots/Tracks/Selected/Parameters/truth_qoverp"},
    };

    std::map<IDPVMDefs::variable, Plot<TH1>>  IDPVMplotsReco;
    std::map<IDPVMDefs::variable, Plot<TH1>>  IDPVMplotsTruth;

    std::map<IDPVMDefs::variable, Plot<TH1D>> distributionsReco;
    std::map<IDPVMDefs::variable, Plot<TH1D>> distributionsTruth;

    for (auto & var : mapIDPVMReco) {
        IDPVMplotsReco.emplace(var.first,
            LoadIDPVMHistogram<TH1F, TH1>(myphysval, mapIDPVMReco.at(var.first))); // nominal reco plot from IDPVM

        IDPVMplotsTruth.emplace(var.first,
            LoadIDPVMHistogram<TH1F, TH1>(myphysval, mapIDPVMTruth.at(var.first))); // nominal truth plot from IDPVM

        auto htemplateReco = IDPVMTemplates::getDistributionHistTemplate(var.first, IDPVMDefs::track); // reco template histogram
        auto varReaderReco = NtupleVarReaderProvider::generateVarReader(var.first, IDPVMDefs::track); // reco variable reader
        PlotFillInstructionWithRef<TH1D, IDPVMTree> fillerReco(IDPVMLabels::getVarName(var.first), // reco plot filler
            [=](TH1D* h, IDPVMTree &t){ h->Fill(varReaderReco(t)); },
            htemplateReco
        );

        auto htemplateTruth = IDPVMTemplates::getDistributionHistTemplate(var.first, IDPVMDefs::truth); // truth template histogram
        auto varReaderTruth = NtupleVarReaderProvider::generateVarReader(var.first, IDPVMDefs::truth); // truth variable reader
        PlotFillInstructionWithRef<TH1D, IDPVMTree> fillerTruth(IDPVMLabels::getVarName(var.first), // truth plot filler
            [=](TH1D* h, IDPVMTree &t){ h->Fill(varReaderTruth(t)); },
            htemplateTruth
        );

        distributionsReco.emplace(var.first,
            Plot<TH1D>(ntuple, IDPVMSelections::passedTrack(), fillerReco)); // reco plot

        distributionsTruth.emplace(var.first,
            Plot<TH1D>(ntuple, IDPVMSelections::forEfficiencyDen(), fillerTruth)); // truth plot
    }

    for (auto & var : mapIDPVMReco) {
        distributionsReco.at(var.first).populate();
        distributionsTruth.at(var.first).populate();
        IDPVMPlotUtils::CompareWithIDPVM(IDPVMplotsReco.at(var.first), distributionsReco.at(var.first), {"IDPVM Ntuple Validation", mapIDPVMReco.at(var.first)});
        IDPVMPlotUtils::CompareWithIDPVM(IDPVMplotsTruth.at(var.first), distributionsTruth.at(var.first), {"IDPVM Ntuple Validation", mapIDPVMTruth.at(var.first)});
    }

    return 0;
}
