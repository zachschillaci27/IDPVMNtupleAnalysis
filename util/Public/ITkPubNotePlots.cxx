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

template <class H> void DrawLHCCResolutionPlots(PlotContent<H> & pc, const std::string & xlabel, std::pair<double, double> minmax = {999, 999}) {
    SetAtlasStyle();
    
    pc.populateAll();
    std::vector<Plot<H>> plots = pc.getPlots();

    CanvasOptions canvasOpts = pc.getCanvasOptions();

    std::shared_ptr<TCanvas> can = std::make_shared<TCanvas>("can","can",canvasOpts.canSizeX(),canvasOpts.canSizeY());

    can->cd();
    auto frame = canvasOpts.drawTopFrame(plots);
    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();
    if (minmax.first != 999) {
        frame->SetMinimum(minmax.first);
        frame->SetMaximum(minmax.second);
    }

    frame->GetXaxis()->SetTitle(xlabel.c_str());

    for (auto & plot : plots) {
        plot->Draw(std::string ("SAME" + (plot.plotFormat().ExtraDrawOpts().isSet ? plot.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
        if (plot.getLegendTitle().find("Run-2") != std::string::npos) {
            plot->GetXaxis()->SetRangeUser(-2.4, 2.4);
        }
    }
    gPad->RedrawAxis();

    std::shared_ptr<TLegend> leg(canvasOpts.drawLegend(plots));
    if (plots.size() > 5) leg->SetNColumns(2);
    canvasOpts.drawLabels(pc.getLabels());
    
    canvasOpts.SaveCanvas(can, pc.getFileName(), pc.getMultiPageFileName());
}

void DrawLHCCFakeRatePlot(const Plot<TProfile> & h_ITk, const Plot<TProfile> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename, const std::string & multiPagePdf="") {
    SetAtlasStyle();

    Plot<TProfile> itk("h_ITk", h_ITk);
    Plot<TProfile> run2("h_Run2", h_Run2);

    itk.applyFormat();
    run2.applyFormat();

    std::shared_ptr<TCanvas> can = std::make_shared<TCanvas>("can","can",canvasOpts.canSizeX(),canvasOpts.canSizeY());
    
    can->cd();
    std::vector<Plot<TProfile>> plots{itk};
    auto frame = canvasOpts.drawTopFrame(plots);
    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();

    frame->SetMinimum(1.e-7);
    frame->SetMaximum(1.5e0);
    frame->GetXaxis()->SetTitle(xlabel.c_str());

    run2->SetLineWidth(3);
    run2->SetLineStyle(kDashed);

    run2->Draw("SAMEHIST][");
    itk->Draw("SAMEPE");

    double ylabel = 0.90;
    PlotUtils::drawAtlas(0.2, ylabel, "Simulation Preliminary");
    for (auto & label : labels) {
        int fontsize = 18;
        ylabel -= 0.05;
        if (label.find("ATLAS-P2") != std::string::npos) { 
            ylabel += 0.01;
            fontsize = 16;
        }
        PlotUtils::drawTLatex(0.2, ylabel, label, fontsize);
    }    

    PlotUtils::adaptLabels(run2());
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{
        PlotUtils::LegendEntry(run2(), run2.getLegendTitle(), run2.getLegendOption()),
        PlotUtils::LegendEntry(itk(), itk.getLegendTitle(), itk.getLegendOption())}, 
        0.60,0.76,0.88,0.92);
    
    canvasOpts.SaveCanvas(can, filename, multiPagePdf);
}

void DrawLHCCPrimaryFakeRatePlot(const Plot<TProfile> & h_ITk, const Plot<TProfile> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename, const std::string & multiPagePdf="") {
    SetAtlasStyle();

    Plot<TProfile> itk("h_ITk", h_ITk);
    Plot<TProfile> run2("h_Run2", h_Run2);

    itk.applyFormat();
    run2.applyFormat();

    std::shared_ptr<TCanvas> can = std::make_shared<TCanvas>("can","can",canvasOpts.canSizeX(),canvasOpts.canSizeY());
    
    can->cd();
    std::vector<Plot<TH1>> plots{Plot<TH1>("", itk->ProjectionX())};
    auto frame = canvasOpts.drawTopFrame(plots);
    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();

    frame->SetMinimum(1.e-7);
    frame->SetMaximum(5.e-2);

    run2->SetLineWidth(3);
    run2->SetLineStyle(kDashed);
    frame->GetXaxis()->SetTitle(xlabel.c_str());

    run2->Draw("SAMEHIST][");
    itk->Draw("SAMEPE");

    double ylabel = 0.90;
    PlotUtils::drawAtlas(0.2, ylabel, "Simulation Preliminary");
    for (auto & label : labels) {
        int fontsize = 18;
        ylabel -= 0.05;
        if (label.find("ATLAS-P2") != std::string::npos) { 
            ylabel += 0.01;
            fontsize = 16;
        }
        PlotUtils::drawTLatex(0.2, ylabel, label, fontsize);
    }    

    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{
        PlotUtils::LegendEntry(run2(), run2.getLegendTitle(), run2.getLegendOption()),
        PlotUtils::LegendEntry(itk(), itk.getLegendTitle(), itk.getLegendOption())}, 
        0.60,0.76,0.88,0.92);
    
    canvasOpts.SaveCanvas(can, filename, multiPagePdf);
}

void DrawLHCCEfficiencyEtaPlot(const Plot<TH1> & h_ITk, const Plot<TH1> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename, const std::string & multiPagePdf="") {
    SetAtlasStyle();

    Plot<TH1> itk("h_ITk", h_ITk);
    Plot<TH1> run2("h_Run2", h_Run2);
    itk.applyFormat();
    run2.applyFormat();

    Plot<TH1> ratio("ratio", itk);
    ratio.setPlotFormat(run2.plotFormat());
    ratio.applyFormat();
    ratio->GetYaxis()->SetTitle("#frac{ITk Layout}{Run-2}");

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

    run2->SetLineWidth(3);
    run2->SetLineStyle(kDashed);
    run2->SetMinimum(0.70);
    run2->SetMaximum(1.015);
    run2->GetYaxis()->SetNdivisions(8,5,0);
    ratio->SetMinimum(0.915);
    ratio->SetMaximum(1.065);
    ratio->GetYaxis()->SetNdivisions(3,5,0);
    ratio->GetXaxis()->SetTitle(xlabel.c_str());
    ratio->GetXaxis()->SetTitleOffset(2.5 * ratio->GetXaxis()->GetTitleOffset());
    ratio->GetYaxis()->SetTitleOffset(1.3 * ratio->GetYaxis()->GetTitleOffset());

    run2->Draw("HIST][");
    itk->Draw("SAME");
    
    double ylabel = 0.88;
    PlotUtils::drawAtlas(0.2, ylabel, "Simulation Preliminary");
    for (auto & label : labels) {
        int fontsize = 18;
        ylabel -= 0.07;
        if (label.find("ATLAS-P2") != std::string::npos) { 
            ylabel += 0.02;
            fontsize = 16;
        }
        PlotUtils::drawTLatex(0.2, ylabel, label, fontsize);
    }    
    
    PlotUtils::adaptLabels(run2());
    PlotUtils::adaptLabels(ratio());
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{
        PlotUtils::LegendEntry(run2(), run2.getLegendTitle(), run2.getLegendOption()),
        PlotUtils::LegendEntry(itk(), itk.getLegendTitle(), itk.getLegendOption())}, 
        0.60,0.79,0.88,0.95);

    mpc.getPad(1)->cd();
    ratio->Draw("");

    PlotUtils::saveCanvas(mpc.getCanvas(), filename);
    PlotUtils::saveCanvasToMultiPagePdfFile(mpc.getCanvas(), multiPagePdf);
}

void DrawLHCCEfficiencyPtPlot(const Plot<TH1> & h_ITk, const Plot<TH1> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename, const std::string & multiPagePdf="") {

    Plot<TH1> itk("h_ITk", h_ITk);
    Plot<TH1> run2("h_Run2", h_Run2);
    itk.applyFormat();
    run2.applyFormat();

    Plot<TH1> ratio("ratio", itk);
    ratio.setPlotFormat(run2.plotFormat());
    ratio.applyFormat();
    ratio->GetYaxis()->SetTitle("#frac{ITk Layout}{Run-2}");

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

    run2->SetLineWidth(3);
    run2->SetLineStyle(kDashed);
    run2->SetMinimum(0.70);
    run2->SetMaximum(1.015);
    run2->GetYaxis()->SetNdivisions(8,5,0);
    ratio->SetMinimum(0.915);
    ratio->SetMaximum(1.065);
    ratio->GetYaxis()->SetNdivisions(3,5,0);
    ratio->GetXaxis()->SetTitle(xlabel.c_str());
    ratio->GetXaxis()->SetTitleOffset(2.5 * ratio->GetXaxis()->GetTitleOffset());
    ratio->GetYaxis()->SetTitleOffset(1.3 * ratio->GetYaxis()->GetTitleOffset());

    run2->Draw("HIST][");
    itk->Draw("SAME");
    
    double ylabel = 0.88;
    PlotUtils::drawAtlas(0.2, ylabel, "Simulation Preliminary");
    for (auto & label : labels) {
        int fontsize = 18;
        ylabel -= 0.07;
        if (label.find("ATLAS-P2") != std::string::npos) { 
            ylabel += 0.02;
            fontsize = 16;
        }
        PlotUtils::drawTLatex(0.2, ylabel, label, fontsize);
    }    
    
    PlotUtils::adaptLabels(run2());
    PlotUtils::adaptLabels(ratio());
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{
        PlotUtils::LegendEntry(run2(), run2.getLegendTitle(), run2.getLegendOption()),
        PlotUtils::LegendEntry(itk(), itk.getLegendTitle(), itk.getLegendOption())}, 
        0.60,0.79,0.88,0.95);

    mpc.getPad(1)->cd();
    ratio->Draw("");

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

    const std::vector<std::string> labels_muons_d0 = {"ITk Layout: ATLAS-P2-ITK-23-00-01", "Single #mu"}; 
    const std::vector<std::string> labels_muons_z0 = {"ITk Layout: ATLAS-P2-ITK-23-00-01", "Single #mu"}; 
    const std::vector<std::string> labels_ttbar_eff    = {"ITk Layout: ATLAS-P2-ITK-23-00-01", "t#bar{t}, p_{T} > 1 GeV"}; 
    const std::vector<std::string> labels_ttbar_fake   = {"ITk Layout: ATLAS-P2-ITK-23-00-01", "t#bar{t}"}; 

    // CanvasOptions opts = CanvasOptions().doAtlasLabel(true).otherLabelStartY(-0.11).otherLabelStepY(0.05).legendStartX(-0.3).legendStartY(-0.2).fontSize(20).axisFontSize(25).labelStatusTag("Simulation Preliminary");
    CanvasOptions opts = CanvasOptions().doAtlasLabel(true).otherLabelStartY(-0.11).otherLabelStepY(0.05).legendStartX(-0.3).legendStartY(-0.2).fontSize(20).labelStatusTag("Simulation Preliminary");
    CanvasOptions opts_muons = CanvasOptions(opts).doLumiLabel(false).logY(true);
    CanvasOptions opts_ttbar = CanvasOptions(opts).labelSqrtsTag("14 TeV").labelLumiTag("HL-LHC");
    CanvasOptions opts_ttbar_log = CanvasOptions(opts_ttbar).logY(true);

    SetAtlasStyle();
    
    const std::string multiPagePdf = "ITkPubNotePlots";
    PlotUtils::startMultiPagePdfFile(multiPagePdf);

    ////////////////////// RESOLUTION //////////////////////

    PlotContent<TH1F> d0ResPlot(
        std::vector<Plot<TH1F>> {
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(run2_sglmu1, d0_vs_eta), "Run-2, p_{T} = 1 GeV", "L", PlotFormat().Color(kBlack).LineStyle(kSolid).ExtraDrawOpts("HIST][")),
             Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(run2_sglmu100, d0_vs_eta), "Run-2, p_{T} = 100 GeV", "L", PlotFormat().Color(kRed).LineStyle(kDashed).ExtraDrawOpts("HIST][")),
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu1, d0_vs_eta), "ITk, p_{T} = 1 GeV", "PL", PlotFormat().Color(kBlue).MarkerStyle(kOpenCircle)),
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu100, d0_vs_eta), "ITk, p_{T} = 100 GeV", "PL", PlotFormat().Color(kGreen+3).MarkerStyle(kOpenSquare)),
        }, labels_muons_d0, "SingleMu-d0Resolution_vs_eta", multiPagePdf, opts_muons);

    DrawLHCCResolutionPlots(d0ResPlot, "true track |#eta|", {2.0, 5.e3});

    PlotContent<TH1F> z0ResPlot(
        std::vector<Plot<TH1F>> {
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(run2_sglmu1, z0_vs_eta), "Run-2, p_{T} = 1 GeV", "L", PlotFormat().Color(kBlack).LineStyle(kSolid).ExtraDrawOpts("HIST][")),
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(run2_sglmu100, z0_vs_eta), "Run-2, p_{T} = 100 GeV", "L", PlotFormat().Color(kRed).LineStyle(kDashed).ExtraDrawOpts("HIST][")),
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu1, z0_vs_eta), "ITk, p_{T} = 1 GeV", "PL", PlotFormat().Color(kBlue).MarkerStyle(kOpenCircle)),
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu100, z0_vs_eta), "ITk, p_{T} = 100 GeV", "PL", PlotFormat().Color(kGreen+3).MarkerStyle(kOpenSquare)),
        }, labels_muons_z0, "SingleMu-z0Resolution_vs_eta", multiPagePdf, opts_muons);
    
    DrawLHCCResolutionPlots(z0ResPlot, "true track |#eta|", {2.0, 3.e5});

    ////////////////////// EFFICIENCY //////////////////////

    DrawLHCCEfficiencyEtaPlot( 
        Plot<TH1>("", LoadIDPVMEfficiency(ttbarmu200, eff_vs_eta), "ITk, <#mu> = 200", "PL", PlotFormat().Color(kBlue).MarkerStyle(kFullCircle).MarkerSize(1.25)),
        Plot<TH1>("", LoadIDPVMEfficiency(run2_ttbarmu20, eff_vs_eta), "Run-2, <#mu> = 20", "L", PlotFormat().Color(kBlack)),
        labels_ttbar_eff, opts_ttbar, "true track |#eta|", "ttbar-efficiency_vs_eta", multiPagePdf);

    DrawLHCCEfficiencyPtPlot( 
        Plot<TH1>("", LoadIDPVMEfficiency(ttbarmu200, eff_vs_pt), "ITk |#eta| < 4.0, <#mu> = 200", "PL", PlotFormat().Color(kBlue).MarkerStyle(kFullCircle).MarkerSize(1.25)),
        Plot<TH1>("", LoadIDPVMEfficiency(run2_ttbarmu20, eff_vs_pt), "Run-2 |#eta| < 2.4, <#mu> = 20", "L", PlotFormat().Color(kBlack)),
        labels_ttbar_eff, opts_ttbar, "true track p_{T} [GeV]", "ttbar-efficiency_vs_pt", multiPagePdf);

    ////////////////////// FAKE-RATES //////////////////////

    DrawLHCCFakeRatePlot( 
        Plot<TProfile>("", LoadIDPVMHistogram<TProfile>(ttbarmu200, fake_vs_eta), "ITk, <#mu> = 200", "PL", PlotFormat().Color(kBlue).MarkerStyle(kFullCircle).MarkerSize(1.25)),
        Plot<TProfile>("", LoadIDPVMHistogram<TProfile>(run2_ttbarmu20, fake_vs_eta), "Run-2, <#mu> = 20", "L", PlotFormat().Color(kBlack)),
        labels_ttbar_fake, opts_ttbar_log, "|#eta|", "ttbar-fakerate_vs_eta", multiPagePdf);

    // DrawLHCCPrimaryFakeRatePlot( 
    //     Plot<TProfile>("", LoadIDPVMHistogram<TProfile>(ttbarmu200, fake_primary_vs_eta), "ITk, <#mu> = 200", "PL", PlotFormat().Color(kBlue).MarkerStyle(kFullCircle).MarkerSize(1.25)),
    //     Plot<TProfile>("", LoadIDPVMHistogram<TProfile>(run2_ttbarmu20, fake_primary_vs_eta), "Run-2, <#mu> = 20", "L", PlotFormat().Color(kBlack)),
    //     labels_ttbar_fake, opts_ttbar_log, "|#eta|", "ttbar-primaryfakerate_vs_eta", multiPagePdf);

    PlotUtils::endMultiPagePdfFile(multiPagePdf);

    return EXIT_SUCCESS;
}
