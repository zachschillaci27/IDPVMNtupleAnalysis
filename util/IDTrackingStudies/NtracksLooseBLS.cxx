#include "NtupleAnalysisUtils/NtupleBranch.h"
#include "NtupleAnalysisUtils/Selection.h"
#include "NtupleAnalysisUtils/PlotFillInstruction.h"
#include "NtupleAnalysisUtils/PlotContent.h"
#include "NtupleAnalysisUtils/Sample.h"
#include "NtupleAnalysisUtils/PlotUtils.h"
#include "NtupleAnalysisUtils/CanvasOptions.h"
#include "NtupleAnalysisUtils/AtlasStyle.h"
#include "NtupleAnalysisUtils/DefaultPlotting.h"
#include "IDPVMNtupleAnalysis/IDPVMUtilities.h"

template <class H> void draw1DNoRatio(PlotContent<H> & pc){
    SetAtlasStyle();

    pc.populateAll();
    std::vector<Plot<H>> plots = pc.getPlots();

    CanvasOptions canvasOpts = pc.getCanvasOptions();

    std::shared_ptr<TCanvas> can = std::make_shared<TCanvas>("can","can",canvasOpts.canSizeX(),canvasOpts.canSizeY());

    can->cd();
    canvasOpts.drawTopFrame(plots);
    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();

    std::shared_ptr<TLegend> leg(canvasOpts.drawLegend(plots));
    if (plots.size() > 5) leg->SetNColumns(2);

    auto labels = pc.getLabels();
    labels.push_back("#bf{Average tracks per event:}");
    for (auto & plot : plots) {
        labels.push_back(Form("%s: %.1f", plot.getLegendTitle().c_str(), plot->GetMean()));
    }
    canvasOpts.drawLabels(labels);

    for (auto & plot : plots){
        plot->Draw(std::string ("SAME" + (plot.plotFormat().ExtraDrawOpts().isSet ? plot.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    }
    gPad->RedrawAxis();

    canvasOpts.SaveCanvas(can, pc.getFileName(), pc.getMultiPageFileName());
}

int main (int, char**) {

    SetAtlasStyle();

    // Baseline (Athena master - circa March 19, 2021)
    const std::string baseline_mu50 = "/home/zschilla/Working/Athena/Tracking/run/NewCutsForBLS/Baseline/runTiming_r22_2017_337833_mu_50/M_output.root";
    const std::string baseline_mu60 = "/home/zschilla/Working/Athena/Tracking/run/NewCutsForBLS/Baseline/runTiming_r22_2017_337833_mu_60/M_output.root";
    const std::string baseline_mu80 = "/home/zschilla/Working/Athena/Tracking/run/NewCutsForBLS/Baseline/runTiming_r22_2018_364485_mu_80.1/M_output.root";

    // Removing cut-levels 15/16 from baseline
    const std::string newCuts_mu50 = "/home/zschilla/Working/Athena/Tracking/run/NewCutsForBLS/NewCuts/runTiming_r22_2017_337833_mu_50/M_output.root";
    const std::string newCuts_mu60 = "/home/zschilla/Working/Athena/Tracking/run/NewCutsForBLS/NewCuts/runTiming_r22_2017_337833_mu_60/M_output.root";
    const std::string newCuts_mu80 = "/home/zschilla/Working/Athena/Tracking/run/NewCutsForBLS/NewCuts/runTiming_r22_2018_364485_mu_80.1/M_output.root";

    // Plots to compare from IDPVM
    // "SquirrelPlots/Tracks/Tracks/ntrack"    // TH1F
    // "SquirrelPlots/Tracks/Tracks/ntracksel" // TH1F
    
    // Baseline plots
    Plot<TH1F> baseline_mu50_ntrack("baseline_mu50_ntrack", 
        LoadIDPVMHistogram<TH1F>(baseline_mu50, "SquirrelPlots/Tracks/Tracks/ntrack"), "Baseline, #LT#mu#GT=50", "F", PremadePlotFormats::Lines1().ExtraDrawOpts("HIST").FillStyle(1001));
    
    Plot<TH1F> baseline_mu60_ntrack("baseline_mu60_ntrack", 
        LoadIDPVMHistogram<TH1F>(baseline_mu60, "SquirrelPlots/Tracks/Tracks/ntrack"), "Baseline, #LT#mu#GT=60", "F", PremadePlotFormats::Lines2().ExtraDrawOpts("HIST").FillStyle(1001));
    
    Plot<TH1F> baseline_mu80_ntrack("baseline_mu80_ntrack", 
        LoadIDPVMHistogram<TH1F>(baseline_mu80, "SquirrelPlots/Tracks/Tracks/ntrack"), "Baseline, #LT#mu#GT=80", "F", PremadePlotFormats::Lines3().ExtraDrawOpts("HIST").FillStyle(1001));

    // New cuts plots
    Plot<TH1F> newCuts_mu50_ntrack("newCuts_mu50_ntrack", 
        LoadIDPVMHistogram<TH1F>(newCuts_mu50, "SquirrelPlots/Tracks/Tracks/ntrack"), "New Cuts, #LT#mu#GT=50", "F", PremadePlotFormats::Lines4().ExtraDrawOpts("HIST").FillStyle(1001));
    
    Plot<TH1F> newCuts_mu60_ntrack("newCuts_mu60_ntrack", 
        LoadIDPVMHistogram<TH1F>(newCuts_mu60, "SquirrelPlots/Tracks/Tracks/ntrack"), "New Cuts, #LT#mu#GT=60", "F", PremadePlotFormats::Lines5().ExtraDrawOpts("HIST").FillStyle(1001));
    
    Plot<TH1F> newCuts_mu80_ntrack("newCuts_mu80_ntrack", 
        LoadIDPVMHistogram<TH1F>(newCuts_mu80, "SquirrelPlots/Tracks/Tracks/ntrack"), "New Cuts, #LT#mu#GT=80", "F", PremadePlotFormats::Lines6().ExtraDrawOpts("HIST").FillStyle(1001));

    // Comparisons
    PlotContent<TH1F> baseline({baseline_mu50_ntrack, baseline_mu60_ntrack, baseline_mu80_ntrack}, {"Release 22.0.30"}, "newCutsForBLS/baseline", "newCutsForBLS/IDPVMnewCutsForBLS");
    PlotContent<TH1F> newCuts({newCuts_mu50_ntrack, newCuts_mu60_ntrack, newCuts_mu80_ntrack},     {"Release 22.0.30"}, "newCutsForBLS/newCuts", "newCutsForBLS/IDPVMnewCutsForBLS");

    PlotContent<TH1F> mu50({baseline_mu50_ntrack, newCuts_mu50_ntrack}, {"Release 22.0.30"}, "newCutsForBLS/mu50", "newCutsForBLS/IDPVMnewCutsForBLS");
    PlotContent<TH1F> mu60({baseline_mu60_ntrack, newCuts_mu60_ntrack}, {"Release 22.0.30"}, "newCutsForBLS/mu60", "newCutsForBLS/IDPVMnewCutsForBLS");
    PlotContent<TH1F> mu80({baseline_mu80_ntrack, newCuts_mu80_ntrack}, {"Release 22.0.30"}, "newCutsForBLS/mu80", "newCutsForBLS/IDPVMnewCutsForBLS");

    PlotUtils::startMultiPagePdfFile("newCutsForBLS/IDPVMnewCutsForBLS");
    draw1DNoRatio(baseline);   
    draw1DNoRatio(newCuts);   
    draw1DNoRatio(mu50);   
    draw1DNoRatio(mu60);   
    draw1DNoRatio(mu80);   
    PlotUtils::endMultiPagePdfFile("newCutsForBLS/IDPVMnewCutsForBLS");

    return EXIT_SUCCESS;
}
