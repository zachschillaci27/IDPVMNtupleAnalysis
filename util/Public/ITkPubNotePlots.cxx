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

void DrawDynamicLabels(const std::vector<std::string> & labels, double xstart = 0.10, double ystart = 0.90) {
    PlotUtils::drawAtlas(xstart, ystart, "Simulation Preliminary");
    for (auto & label : labels) {
        int fontsize = (label.find("ATLAS-P2") == std::string::npos) ? 18   : 16;
        ystart      -= (label.find("ATLAS-P2") == std::string::npos) ? 0.07 : 0.05;
        PlotUtils::drawTLatex(xstart, ystart, label, fontsize);
    }       
}

template <class H> void FormatAxisLabelSize(H & plot, bool isRatio = false) {
    plot->GetXaxis()->SetTitleFont(43);
    plot->GetYaxis()->SetTitleFont(43);
    plot->GetXaxis()->SetLabelFont(43);
    plot->GetYaxis()->SetLabelFont(43);
    plot->GetXaxis()->SetTitleSize(24) ; 
    plot->GetYaxis()->SetTitleSize(isRatio ? 22 : 24); 
    plot->GetXaxis()->SetLabelSize(22); 
    plot->GetYaxis()->SetLabelSize(22); 
    plot->GetXaxis()->SetTitleOffset(isRatio ? 2.25 : 1.25);
    plot->GetYaxis()->SetTitleOffset(1.25);
}

template <class H> void DrawLHCCResolutionPlots(PlotContent<H> & pc, const std::string & xlabel, std::pair<double, double> minmax = {999, 999}) {
    SetAtlasStyle();
    
    pc.populateAll();
    auto plots  = pc.getPlots();
    auto ratios = pc.getRatios();

    CanvasOptions canvasOpts = pc.getCanvasOptions();
    
    auto mpc = PlotUtils::prepareTwoPadCanvas("", 0.4, canvasOpts.canSizeX(), canvasOpts.canSizeY());

    mpc.getPad(0)->cd();
    auto frame = canvasOpts.drawTopFrame(plots);
    FormatAxisLabelSize(frame);

    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();
    
    if (minmax.first != minmax.second) {
        frame->SetMinimum(minmax.first);
        frame->SetMaximum(minmax.second);
    }

    for (auto & plot : plots) {
        plot->Draw(std::string ("SAME" + (plot.plotFormat().ExtraDrawOpts().isSet ? plot.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
        if (plot.getLegendTitle().find("Run-2") != std::string::npos) {
            plot->GetXaxis()->SetRangeUser(-2.4, 2.4);
        }
    }
    gPad->RedrawAxis();

    DrawDynamicLabels(pc.getLabels(), 0.15, 0.88);
    canvasOpts.drawLegend(plots);

    mpc.getPad(1)->cd();
    auto rframe = canvasOpts.drawRatioFrame(ratios);
    FormatAxisLabelSize(rframe, true);
    rframe->GetXaxis()->SetTitle(xlabel.c_str());

    if (canvasOpts.logX())     gPad->SetLogx();
    if (canvasOpts.logRatio()) gPad->SetLogy();
    
    rframe->SetMinimum(0.0);
    rframe->SetMaximum(1.49);

    for (auto & ratio : ratios) {
        ratio->Draw(std::string ("SAME" + (ratio.plotFormat().ExtraDrawOpts().isSet ? ratio.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    }
    gPad->RedrawAxis();

    canvasOpts.SaveCanvas(mpc.getCanvas(), pc.getFileName(), pc.getMultiPageFileName());
}

void DrawLHCCFakeRatePlot(const Plot<TProfile> & h_ITk, const Plot<TProfile> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename, const std::string & multiPagePdf="") {
    SetAtlasStyle();

    Plot<TProfile> itk("h_ITk", h_ITk);
    itk.applyFormat();
    
    Plot<TProfile> run2("h_Run2", h_Run2);
    run2.applyFormat();

    std::shared_ptr<TCanvas> can = std::make_shared<TCanvas>("can","can",canvasOpts.canSizeX(),canvasOpts.canSizeY());
    
    can->cd();
    std::vector<Plot<TProfile>> plots{itk};
    auto frame = canvasOpts.drawTopFrame(plots);
    FormatAxisLabelSize(frame);
    frame->GetXaxis()->SetTitle(xlabel.c_str());

    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();
    
    frame->SetMinimum(1.e-7);
    frame->SetMaximum(1.5e0);

    run2->Draw("SAMEHIST][");
    itk->Draw("SAMEPE");

    DrawDynamicLabels(labels, 0.20, 0.90);
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{
        PlotUtils::LegendEntry(run2(), run2.getLegendTitle(), run2.getLegendOption()),
        PlotUtils::LegendEntry(itk(),  itk.getLegendTitle(),  itk.getLegendOption())}, 
        0.60,0.76,0.88,0.92);
    
    canvasOpts.SaveCanvas(can, filename, multiPagePdf);
}

void DrawLHCCPrimaryFakeRatePlot(const Plot<TProfile> & h_ITk, const Plot<TProfile> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename, const std::string & multiPagePdf="") {
    SetAtlasStyle();

    Plot<TProfile> itk("h_ITk", h_ITk);
    itk.applyFormat();
    
    Plot<TProfile> run2("h_Run2", h_Run2);
    run2.applyFormat();
    run2->GetXaxis()->SetTitle(xlabel.c_str());

    std::shared_ptr<TCanvas> can = std::make_shared<TCanvas>("can","can",canvasOpts.canSizeX(),canvasOpts.canSizeY());
    
    can->cd();

    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();

    run2->SetMinimum(1.e-7);
    run2->SetMaximum(5.e-2);

    run2->Draw("SAMEHIST][");
    itk->Draw("SAMEPE");
    FormatAxisLabelSize(run2);

    DrawDynamicLabels(labels, 0.15, 0.90);
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{
        PlotUtils::LegendEntry(run2(), run2.getLegendTitle(), run2.getLegendOption()),
        PlotUtils::LegendEntry(itk(),  itk.getLegendTitle(),  itk.getLegendOption())}, 
        0.60,0.76,0.88,0.92);
    
    canvasOpts.SaveCanvas(can, filename, multiPagePdf);
}

void DrawLHCCEfficiencyEtaPlot(const Plot<TH1> & h_ITk, const Plot<TH1> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename, const std::string & multiPagePdf="") {
    SetAtlasStyle();

    Plot<TH1> itk("h_ITk", h_ITk);
    itk.applyFormat();
    
    Plot<TH1> run2("h_Run2", h_Run2);
    run2.applyFormat();

    Plot<TH1> ratio("ratio", itk);
    ratio.setPlotFormat(itk.plotFormat());
    ratio.applyFormat();
    ratio->GetXaxis()->SetTitle(xlabel.c_str());
    ratio->GetYaxis()->SetTitle("Ratio w.r.t. Run-2");

    for (int k = 1; k < ratio->GetNbinsX() + 1; ++k) {
        if (run2->GetBinContent(k) != 0.) {
            ratio->SetBinContent(k, itk->GetBinContent(k) / run2->GetBinContent(k));
            ratio->SetBinError(k, sqrt(pow(itk->GetBinError(k) / run2->GetBinContent(k), 2) + pow(run2->GetBinError(k) * itk->GetBinContent(k) / pow(run2->GetBinContent(k), 2), 2)));
        }
        else {
            ratio->SetBinContent(k, 0.);
            ratio->SetBinError(k, 0.);
        }
    }

    auto mpc = PlotUtils::prepareTwoPadCanvas();
    mpc.getPad(0)->cd();

    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();

    run2->SetMinimum(0.64);
    run2->SetMaximum(1.014);
    run2->GetYaxis()->SetNdivisions(8,5,0);
    ratio->SetMinimum(0.915);
    ratio->SetMaximum(1.065);
    ratio->GetYaxis()->SetNdivisions(3,5,0);

    run2->Draw("HIST][");
    itk->Draw("SAME");
    FormatAxisLabelSize(run2);
    
    DrawDynamicLabels(labels, 0.15, 0.88);
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{
        PlotUtils::LegendEntry(run2(), run2.getLegendTitle(), run2.getLegendOption()),
        PlotUtils::LegendEntry(itk(),  itk.getLegendTitle(),  itk.getLegendOption())}, 
        0.60,0.79,0.88,0.95);

    mpc.getPad(1)->cd();
    ratio->Draw("");
    FormatAxisLabelSize(ratio, true);

    PlotUtils::saveCanvas(mpc.getCanvas(), filename);
    PlotUtils::saveCanvasToMultiPagePdfFile(mpc.getCanvas(), multiPagePdf);
}

void DrawLHCCEfficiencyPtPlot(const Plot<TH1> & h_ITk, const Plot<TH1> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename, const std::string & multiPagePdf="") {

    Plot<TH1> itk("h_ITk", h_ITk);
    itk.applyFormat();
    
    Plot<TH1> run2("h_Run2", h_Run2);
    run2.applyFormat();

    Plot<TH1> ratio("ratio", itk);
    ratio.setPlotFormat(itk.plotFormat());
    ratio.applyFormat();
    ratio->GetXaxis()->SetTitle(xlabel.c_str());
    ratio->GetYaxis()->SetTitle("Ratio w.r.t. Run-2");

    for (int k = 1; k < ratio->GetNbinsX() + 1; ++k) {
        if (run2->GetBinContent(k) != 0.) {
            ratio->SetBinContent(k, itk->GetBinContent(k) / run2->GetBinContent(k));
            ratio->SetBinError(k, sqrt(pow(itk->GetBinError(k) / run2->GetBinContent(k), 2) + pow(run2->GetBinError(k) * itk->GetBinContent(k) / pow(run2->GetBinContent(k), 2), 2)));
        }
        else {
            ratio->SetBinContent(k, 0.);
            ratio->SetBinError(k, 0.);
        }
    }

    auto mpc = PlotUtils::prepareTwoPadCanvas();
    mpc.getPad(0)->cd();

    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();

    run2->SetMinimum(0.78);
    run2->SetMaximum(1.015);
    run2->GetYaxis()->SetNdivisions(8,5,0);
    ratio->SetMinimum(0.915);
    ratio->SetMaximum(1.065);
    ratio->GetYaxis()->SetNdivisions(3,5,0);

    run2->Draw("HIST][");
    itk->Draw("SAME");
    FormatAxisLabelSize(run2);

    DrawDynamicLabels(labels, 0.15, 0.88);
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{
        PlotUtils::LegendEntry(run2(), run2.getLegendTitle(), run2.getLegendOption()),
        PlotUtils::LegendEntry(itk(), itk.getLegendTitle(), itk.getLegendOption())}, 
        0.60,0.79,0.88,0.95);

    mpc.getPad(1)->cd();
    ratio->Draw("");
    FormatAxisLabelSize(ratio, true);

    PlotUtils::saveCanvas(mpc.getCanvas(), filename);
    PlotUtils::saveCanvasToMultiPagePdfFile(mpc.getCanvas(), multiPagePdf);
}

int main (int, char**) {

    // ATLAS-P2-ITK-23-00-01
    const std::string sglmu1     = "/scratch/Datasets/ANA-IDTR-2020-01/ATLAS-P2-ITK-23-00-01/sglmu1_PU0_IDPVM_r12350.root";
    const std::string sglmu100   = "/scratch/Datasets/ANA-IDTR-2020-01/ATLAS-P2-ITK-23-00-01/sglmu100_PU0_IDPVM_r12350.root";
    const std::string ttbarmu200 = "/scratch/Datasets/ANA-IDTR-2020-01/ATLAS-P2-ITK-23-00-01/ttbar_PU200_IDPVM_r12351.root";

    // Run-2
    const std::string run2_sglmu1     = "/scratch/Datasets/ANA-IDTR-2020-01/Run2/MyPhysVal.sglmu1.mu0.run2.root";
    const std::string run2_sglmu100   = "/scratch/Datasets/ANA-IDTR-2020-01/Run2/MyPhysVal.sglmu100.mu0.run2.root";
    const std::string run2_ttbarmu20  = "/scratch/Datasets/ANA-IDTR-2020-01/Run2/MyPhysVal.tbbar.mu20.eta0to2p4.run2.root";

    // Resolutions - TH1F
    const std::string d0_vs_eta = "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0resolutionRMS_vs_eta";
    const std::string z0_vs_eta = "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0resolutionRMS_vs_eta";
    const std::string pt_vs_eta = "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverptresolutionRMS_vs_eta";
    // Efficiency - TEfficiency
    const std::string eff_vs_eta = "IDPerformanceMon/Tracks/SelectedGoodTracks/trackeff_vs_eta";
    const std::string eff_vs_pt  = "IDPerformanceMon/Tracks/SelectedGoodTracks/trackeff_vs_pt";
    // Fakes - TProfile
    const std::string fake_primary_vs_eta = "IDPerformanceMon/Tracks/SelectedFakeTracks/fakeetaPrimary";
    const std::string fake_vs_eta         = "IDPerformanceMon/Tracks/SelectedFakeTracks/track_fakerate_vs_eta";
    const std::string fake_vs_pt          = "IDPerformanceMon/Tracks/SelectedFakeTracks/track_fakerate_vs_pt";

    // Labels
    const std::vector<std::string> labels_muons_pt1   = {"ITk Layout: ATLAS-P2-ITK-23-00-01", "Single #mu, p_{T} = 1 GeV"}; 
    const std::vector<std::string> labels_muons_pt100 = {"ITk Layout: ATLAS-P2-ITK-23-00-01", "Single #mu, p_{T} = 100 GeV"}; 
    const std::vector<std::string> labels_ttbar_eta   = {"ITk Layout: ATLAS-P2-ITK-23-00-01", "t#bar{t}, p_{T} > 1 GeV"}; 
    const std::vector<std::string> labels_ttbar_pt    = {"ITk Layout: ATLAS-P2-ITK-23-00-01", "t#bar{t}"}; 
    
    // PlotFormats
    PlotFormat formatRun2 = PlotFormat().Color(kBlack).LineStyle(kDashed).LineWidth(3).ExtraDrawOpts("HIST][");
    PlotFormat formatITk  = PlotFormat().Color(kBlue).MarkerStyle(kOpenCircle).MarkerSize(1.25);

    // CanvasOptions
    CanvasOptions opts = CanvasOptions().doAtlasLabel(true).otherLabelStartY(-0.11).otherLabelStepY(0.05).legendStartX(-0.3).legendStartY(-0.2).fontSize(20).labelStatusTag("Simulation Preliminary").ratioAxisTitle("Ratio w.r.t. Run-2");
    CanvasOptions opts_muons = CanvasOptions(opts).doLumiLabel(false).logY(true);
    CanvasOptions opts_ttbar = CanvasOptions(opts).labelSqrtsTag("14 TeV").labelLumiTag("HL-LHC");
    CanvasOptions opts_ttbar_log = CanvasOptions(opts_ttbar).logY(true);

    SetAtlasStyle();
    
    const std::string multiPagePdf = "ITkPubNotePlots";
    PlotUtils::startMultiPagePdfFile(multiPagePdf);

    ////////////////////// RESOLUTION //////////////////////
    
    // d0
    PlotContent<TH1F> d0ResPlotSingleMu1(
        std::vector<Plot<TH1F>> {
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(run2_sglmu1, d0_vs_eta), "Run-2", "L", formatRun2),
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu1,      d0_vs_eta), "ITk",  "PL", formatITk),
        }, labels_muons_pt1, "SingleMu1-d0Resolution_vs_eta", multiPagePdf, opts_muons);

    DrawLHCCResolutionPlots(d0ResPlotSingleMu1, "true track |#eta|", {7.e0, 5.e3});

    PlotContent<TH1F> d0ResPlotSingleMu100(
        std::vector<Plot<TH1F>> {
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(run2_sglmu100, d0_vs_eta), "Run-2", "L", formatRun2),
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu100,      d0_vs_eta), "ITk",  "PL", formatITk),
        }, labels_muons_pt100, "SingleMu100-d0Resolution_vs_eta", multiPagePdf, opts_muons);

    DrawLHCCResolutionPlots(d0ResPlotSingleMu100, "true track |#eta|", {7.e-1, 1.e3});

    // z0
    PlotContent<TH1F> z0ResPlotSingleMu1(
        std::vector<Plot<TH1F>> {
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(run2_sglmu1, z0_vs_eta), "Run-2", "L", formatRun2),
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu1,      z0_vs_eta), "ITk",  "PL", formatITk),
        }, labels_muons_pt1, "SingleMu1-z0Resolution_vs_eta", multiPagePdf, opts_muons);

    DrawLHCCResolutionPlots(z0ResPlotSingleMu1, "true track |#eta|", {7.e0, 5.e5});

    PlotContent<TH1F> z0ResPlotSingleMu100(
        std::vector<Plot<TH1F>> {
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(run2_sglmu100, z0_vs_eta), "Run-2", "L", formatRun2),
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu100,      z0_vs_eta), "ITk",  "PL", formatITk),
        }, labels_muons_pt100, "SingleMu100-d0Resolution_vs_eta", multiPagePdf, opts_muons);

    DrawLHCCResolutionPlots(z0ResPlotSingleMu100, "true track |#eta|", {7.e-1, 1.e4});

    ////////////////////// EFFICIENCY //////////////////////

    DrawLHCCEfficiencyEtaPlot( 
        Plot<TH1>("", LoadIDPVMEfficiency(ttbarmu200,     eff_vs_eta), "ITk, <#mu> = 200", "PL", formatITk),
        Plot<TH1>("", LoadIDPVMEfficiency(run2_ttbarmu20, eff_vs_eta), "Run-2, <#mu> = 20", "L", formatRun2),
        labels_ttbar_eta, opts_ttbar, "true track |#eta|", "ttbar-efficiency_vs_eta", multiPagePdf);

    DrawLHCCEfficiencyPtPlot( 
        Plot<TH1>("", LoadIDPVMEfficiency(ttbarmu200,     eff_vs_pt), "ITk |#eta| < 4.0, <#mu> = 200", "PL", formatITk),
        Plot<TH1>("", LoadIDPVMEfficiency(run2_ttbarmu20, eff_vs_pt), "Run-2 |#eta| < 2.4, <#mu> = 20", "L", formatRun2),
        labels_ttbar_pt, opts_ttbar, "true track p_{T} [GeV]", "ttbar-efficiency_vs_pt", multiPagePdf);

    ////////////////////// FAKE-RATES //////////////////////

    DrawLHCCFakeRatePlot( 
        Plot<TProfile>("", LoadIDPVMHistogram<TProfile>(ttbarmu200,     fake_vs_eta), "ITk, <#mu> = 200", "PL", formatITk),
        Plot<TProfile>("", LoadIDPVMHistogram<TProfile>(run2_ttbarmu20, fake_vs_eta), "Run-2, <#mu> = 20", "L", formatRun2),
        labels_ttbar_eta, opts_ttbar_log, "|#eta|", "ttbar-fakerate_vs_eta", multiPagePdf);

    PlotUtils::endMultiPagePdfFile(multiPagePdf);

    return EXIT_SUCCESS;
}
