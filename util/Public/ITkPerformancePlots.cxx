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
    std::vector<Plot<H>> plots_raw = pc.getPlots();

    std::vector<Plot<H>> plots;
    for (auto & raw : plots_raw) {
        plots.push_back(SymmetrizeResolution(raw));
    }
    
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

    frame->GetXaxis()->SetRangeUser(0.0, 4.0);
    frame->GetXaxis()->SetTitle(xlabel.c_str());

    for (auto & plot : plots) {
        plot->Draw(std::string ("SAME" + (plot.plotFormat().ExtraDrawOpts().isSet ? plot.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    }
    gPad->RedrawAxis();

    std::shared_ptr<TLegend> leg(canvasOpts.drawLegend(plots));
    if (plots.size() > 5) leg->SetNColumns(2);
    canvasOpts.drawLabels(pc.getLabels());
    
    canvasOpts.SaveCanvas(can, pc.getFileName(), pc.getMultiPageFileName());
}

void DrawLHCCFakeRatePlot(const Plot<TProfile> & h_ITk, const Plot<TH1> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename) {
    SetAtlasStyle();

    Plot<TProfile> itk("h_ITk", h_ITk);
    Plot<TH1> run2("h_Run2", h_Run2);

    itk.applyFormat();
    run2.applyFormat();

    Plot<TProfile> itk_sym = SymmetrizeProfile(itk);

    std::shared_ptr<TCanvas> can = std::make_shared<TCanvas>("can","can",canvasOpts.canSizeX(),canvasOpts.canSizeY());
    
    can->cd();
    std::vector<Plot<TProfile>> plots{itk_sym};
    auto frame = canvasOpts.drawTopFrame(plots);
    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();

    frame->SetMinimum(1.e-7);
    frame->SetMaximum(1.5e0);
    frame->GetXaxis()->SetRangeUser(0.0, 4.0);
    frame->GetXaxis()->SetTitle(xlabel.c_str());

    run2->SetLineWidth(3);
    run2->SetLineStyle(kDashed);

    run2->Draw("SAMEHIST][");
    itk_sym->Draw("SAMEPE");

    // itk->Draw("SAMEPE");
    // itk->SetLineColor(kGreen);
    // itk->SetMarkerColor(kGreen);
    // itk->SetMarkerStyle(kOpenCircle);

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
        PlotUtils::LegendEntry(itk_sym(), itk_sym.getLegendTitle(), itk_sym.getLegendOption())}, 
        0.60,0.76,0.88,0.92);
    
    canvasOpts.SaveCanvas(can, filename);
}

void DrawLHCCPrimaryFakeRatePlot(const Plot<TEfficiency> & h_ITk, const Plot<TEfficiency> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename) {
    SetAtlasStyle();

    Plot<TEfficiency> itk("h_ITk", h_ITk);
    Plot<TEfficiency> run2("h_Run2", h_Run2);

    itk.applyFormat();
    run2.applyFormat();

    Plot<TEfficiency> itk_rebin = RebinEfficiency(itk, 4);
    Plot<TEfficiency> itk_sym = SymmetrizeEfficiency(itk_rebin);

    Plot<TEfficiency> run2_rebin = RebinEfficiency(run2, 4);
    Plot<TEfficiency> run2_sym_eff = SymmetrizeEfficiency(run2_rebin);
    Plot<TH1> run2_sym = ConvertEfficiencyToTH1(run2_sym_eff);

    std::shared_ptr<TCanvas> can = std::make_shared<TCanvas>("can","can",canvasOpts.canSizeX(),canvasOpts.canSizeY());
    
    can->cd();
    std::vector<Plot<TH1>> plots{Plot<TH1>("", itk_sym->GetCopyTotalHisto())};
    auto frame = canvasOpts.drawTopFrame(plots);
    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();

    frame->SetMinimum(1.e-7);
    frame->SetMaximum(5.e-2);
    frame->GetXaxis()->SetRangeUser(0.0, 4.0);

    run2_sym->SetLineWidth(3);
    run2_sym->SetLineStyle(kDashed);
    frame->GetXaxis()->SetTitle(xlabel.c_str());

    run2_sym->Draw("SAMEHIST][");
    itk_sym->Draw("SAMEPE");

    // run2->Draw("SAME");
    // run2->SetLineColor(kViolet);
    // run2->SetMarkerColor(kViolet);
    // itk->Draw("SAME");
    // itk->SetLineColor(kGreen);
    // itk->SetMarkerColor(kGreen);
    // itk->SetMarkerStyle(kOpenCircle);

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
        PlotUtils::LegendEntry(run2_sym(), run2_sym.getLegendTitle(), run2_sym.getLegendOption()),
        PlotUtils::LegendEntry(itk_sym(), itk_sym.getLegendTitle(), itk_sym.getLegendOption())}, 
        0.60,0.76,0.88,0.92);
    
    canvasOpts.SaveCanvas(can, filename);
}

void DrawLHCCEfficiencyEtaPlot(const Plot<TEfficiency> & h_ITk, const Plot<TProfile> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename) {
    SetAtlasStyle();

    Plot<TEfficiency> itk("h_ITk", h_ITk);
    Plot<TProfile> run2("h_Run2", h_Run2);

    itk.applyFormat();
    run2.applyFormat();

    Plot<TEfficiency> itk_sym = SymmetrizeEfficiency(itk);
    Plot<TProfile> run2_sym = SymmetrizeProfile(run2);

    Plot<TH1D> ratio("ratio", dynamic_cast<TH1D*>(itk_sym->GetTotalHistogram()->Clone()));
    ratio.setPlotFormat(run2_sym.plotFormat());
    ratio.applyFormat();
    ratio->GetYaxis()->SetTitle("#frac{ITk Layout}{Run-2}");

    for (int k = 1; k < run2_sym->GetNbinsX() + 1; ++k) {
        if (run2_sym->GetBinContent(k) != 0.) {
            ratio->SetBinContent(k, itk_sym->GetEfficiency(k) / run2_sym->GetBinContent(k));
            double err_it = 0.5 * (itk_sym->GetEfficiencyErrorLow(k) + itk_sym->GetEfficiencyErrorUp(k));
            ratio->SetBinError(k, sqrt(pow(err_it / run2_sym->GetBinContent(k), 2) + pow(run2_sym->GetBinError(k) * itk_sym->GetEfficiency(k) / pow(run2_sym->GetBinContent(k), 2), 2)));
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

    run2_sym->SetLineWidth(3);
    run2_sym->SetLineStyle(kDashed);
    run2_sym->SetMinimum(0.79);
    run2_sym->SetMaximum(1.009);
    run2_sym->GetXaxis()->SetRangeUser(0.0, 4.0);
    run2_sym->GetYaxis()->SetNdivisions(8,5,0);
    ratio->SetMinimum(0.925);
    ratio->SetMaximum(1.075);
    ratio->GetXaxis()->SetRangeUser(0.0, 4.0);
    ratio->GetYaxis()->SetNdivisions(3,5,0);
    ratio->GetXaxis()->SetTitle(xlabel.c_str());
    ratio->GetXaxis()->SetTitleOffset(2.5 * ratio->GetXaxis()->GetTitleOffset());
    ratio->GetYaxis()->SetTitleOffset(1.3 * ratio->GetYaxis()->GetTitleOffset());


    run2_sym->Draw("HIST][");
    itk_sym->Draw("SAME");
    
    // run2->Draw("SAMEHIST][");
    // run2->SetLineColor(kViolet);
    // run2->SetMarkerColor(kViolet);
    // itk->Draw("SAME");
    // itk->SetLineColor(kGreen);
    // itk->SetMarkerColor(kGreen);
    // itk->SetMarkerStyle(kOpenCircle);

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
    
    PlotUtils::adaptLabels(run2_sym());
    PlotUtils::adaptLabels(ratio());
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{
        PlotUtils::LegendEntry(run2_sym(), run2_sym.getLegendTitle(), run2_sym.getLegendOption()),
        PlotUtils::LegendEntry(itk_sym(), itk_sym.getLegendTitle(), itk_sym.getLegendOption())}, 
        0.60,0.79,0.88,0.95);

    mpc.getPad(1)->cd();
    ratio->Draw("");

    PlotUtils::saveCanvas(mpc.getCanvas(), filename);
}

template<class HistoType> void DrawLHCCEfficiencyPtPlot(const Plot<TEfficiency> & h_ITk, const Plot<HistoType> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename) {

    Plot<TEfficiency> itk("h_ITk", h_ITk);
    Plot<HistoType> run2("h_Run2", h_Run2);
    itk.applyFormat();
    run2.applyFormat();

    Plot<TH1D> ratio("ratio", dynamic_cast<TH1D*>(itk->GetTotalHistogram()->Clone()));
    ratio.setPlotFormat(run2.plotFormat());
    ratio.applyFormat();
    ratio->GetYaxis()->SetTitle("#frac{ITk Layout}{Run-2}");

    for (int k = 1; k < run2->GetNbinsX() + 1; ++k) {
        ratio->SetBinContent(k, itk->GetEfficiency(k) / run2->GetBinContent(k));
        double err_it = 0.5 * (itk->GetEfficiencyErrorLow(k) + itk->GetEfficiencyErrorUp(k));
        ratio->SetBinError(k, sqrt(pow(err_it / run2->GetBinContent(k), 2) + pow(run2->GetBinError(k) * itk->GetEfficiency(k) / pow(run2->GetBinContent(k), 2), 2)));
    }

    auto mpc = PlotUtils::prepareTwoPadCanvas();
    mpc.getPad(0)->cd();
    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();

    run2->SetLineWidth(3);
    run2->SetLineStyle(kDashed);
    run2->SetMinimum(0.83);
    run2->SetMaximum(1.009);
    run2->GetYaxis()->SetNdivisions(8,5,0);
    ratio->SetMinimum(0.925);
    ratio->SetMaximum(1.075);
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
}

int main (int, char**) {

    const std::string run2_ttbar_eff = "/Users/zschillaci/CERN/Working/Datasets/Tracking/Productions/LHCC2020_r20p20_Step3p1/Run2/run2_ttbar_NoCuts.root";
    const std::string run2_ttbar_fake = "/Users/zschillaci/CERN/Working/Datasets/Tracking/Productions/LHCC2020_r20p20_Step3p1/Run2/run2_ttbar_mu20_Tight.root";

    const std::string sglmu1 = "/Users/zschillaci/CERN/Working/Datasets/Tracking/Productions/LHCC2020_r20p20_Step3p1/sglmu1/MyPhysVal.root";
    const std::string sglmu10 = "/Users/zschillaci/CERN/Working/Datasets/Tracking/Productions/LHCC2020_r20p20_Step3p1/sglmu10/MyPhysVal.root";
    const std::string sglmu100 = "/Users/zschillaci/CERN/Working/Datasets/Tracking/Productions/LHCC2020_r20p20_Step3p1/sglmu100/MyPhysVal.root";

    const std::string ttbarmu0 = "/Users/zschillaci/CERN/Working/Datasets/Tracking/Productions/LHCC2020_r20p20_Step3p1/ttbarmu0/MyPhysVal.root";
    const std::string ttbarmu200 = "/Users/zschillaci/CERN/Working/Datasets/Tracking/Productions/LHCC2020_r20p20_Step3p1/ttbarmu200/MyPhysVal.root";

    const std::string d0_vs_eta = "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/d0resolutionRMS_vs_eta";
    const std::string z0_vs_eta = "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0resolutionRMS_vs_eta";
    const std::string pt_vs_eta = "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/qoverptresolutionRMS_vs_eta";
    
    const std::string eff_vs_eta = "IDPerformanceMon/Tracks/SelectedGoodTracks/trackeff_vs_eta";
    const std::string eff_vs_pt = "IDPerformanceMon/Tracks/SelectedGoodTracks/trackeff_vs_pt";

    const std::string fake_primary_vs_eta = "IDPerformanceMon/Tracks/SelectedFakeTracks/fakeetaPrimary";
    const std::string fake_vs_eta = "IDPerformanceMon/Tracks/SelectedFakeTracks/track_fakerate_vs_eta";
    const std::string fake_vs_pt = "IDPerformanceMon/Tracks/SelectedFakeTracks/track_fakerate_vs_pt";

    const std::vector<std::string> labels_muons_d0 = {"ITk Layout", "ATLAS-P2-ITK-17-04-02", "Single #mu", "d_{0} Resolution"}; 
    const std::vector<std::string> labels_muons_z0 = {"ITk Layout", "ATLAS-P2-ITK-17-04-02", "Single #mu", "z_{0} Resolution"}; 

    const std::vector<std::string> labels_ttbar = {"ITk Layout", "ATLAS-P2-ITK-17-04-02", "t#bar{t}, p_{T} > 1 GeV"}; 

    // CanvasOptions opts = CanvasOptions().doAtlasLabel(true).otherLabelStartY(-0.11).otherLabelStepY(0.05).legendStartX(-0.3).legendStartY(-0.2).fontSize(20).axisFontSize(25).labelStatusTag("Simulation Preliminary");
    CanvasOptions opts = CanvasOptions().doAtlasLabel(true).otherLabelStartY(-0.11).otherLabelStepY(0.05).legendStartX(-0.3).legendStartY(-0.2).fontSize(20).labelStatusTag("Simulation Preliminary");
    CanvasOptions opts_muons = CanvasOptions(opts).doLumiLabel(false).logY(true);
    CanvasOptions opts_ttbar = CanvasOptions(opts).labelSqrtsTag("14 TeV").labelLumiTag("HL-LHC");
    CanvasOptions opts_ttbar_log = CanvasOptions(opts_ttbar).logY(true);

    SetAtlasStyle();
    
    ////////////////////// RESOLUTION //////////////////////

    PlotContent<TH1F> d0ResPlot(
        std::vector<Plot<TH1F>> {
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu1, d0_vs_eta), "p_{T} = 1 GeV", "PL", PlotFormat().Color(kRed).MarkerStyle(kOpenCircle)),
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu10, d0_vs_eta), "p_{T} = 10 GeV", "PL", PlotFormat().Color(kViolet+6).MarkerStyle(kFullCircle)),
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu100, d0_vs_eta), "p_{T} = 100 GeV", "PL", PlotFormat().Color(kBlue).MarkerStyle(kFullSquare)),
        }, labels_muons_d0, "SingleMu-d0Resolution_vs_eta", "", opts_muons);

    DrawLHCCResolutionPlots(d0ResPlot, "true track |#eta|", {2.0, 5.e3});

    PlotContent<TH1F> z0ResPlot(
        std::vector<Plot<TH1F>> {
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu1, z0_vs_eta), "p_{T} = 1 GeV", "PL", PlotFormat().Color(kRed).MarkerStyle(kOpenCircle)),
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu10, z0_vs_eta), "p_{T} = 10 GeV", "PL", PlotFormat().Color(kViolet+6).MarkerStyle(kFullCircle)),
            Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu100, z0_vs_eta), "p_{T} = 100 GeV", "PL", PlotFormat().Color(kBlue).MarkerStyle(kFullSquare)),
        }, labels_muons_z0, "SingleMu-z0Resolution_vs_eta", "", opts_muons);
    
    DrawLHCCResolutionPlots(z0ResPlot, "true track |#eta|", {2.0, 3.e5});

    ////////////////////// EFFICIENCY //////////////////////

    DrawLHCCEfficiencyEtaPlot( 
        Plot<TEfficiency>("", LoadIDPVMHistogram<TEfficiency>(ttbarmu200, eff_vs_eta), "ITk, <#mu> = 200", "PL", PlotFormat().Color(kBlue).MarkerStyle(kFullCircle).MarkerSize(1.25)),
        Plot<TProfile>("", LoadIDPVMHistogram<TProfile>(run2_ttbar_eff, eff_vs_eta), "Run-2, <#mu> = 20", "L", PlotFormat().Color(kBlack).MarkerStyle(kOpenSquare).MarkerSize(1.25)),
        labels_ttbar, opts_ttbar, "true track |#eta|", "ttbar-efficiency_vs_eta");

    DrawLHCCEfficiencyPtPlot( 
        Plot<TEfficiency>("", LoadIDPVMHistogram<TEfficiency>(ttbarmu200, eff_vs_pt), "ITk  |#eta| < 4.0, <#mu> = 200", "PL", PlotFormat().Color(kBlue).MarkerStyle(kFullCircle).MarkerSize(1.25)),
        Plot<TProfile>("", LoadIDPVMHistogram<TProfile>(run2_ttbar_eff, eff_vs_pt), "Run-2 |#eta| < 2.4, <#mu> = 20", "L", PlotFormat().Color(kBlack).MarkerStyle(kOpenSquare).MarkerSize(1.25)),
        labels_ttbar, opts_ttbar, "true track p_{T} [GeV]", "ttbar-efficiency_vs_pt");

    ////////////////////// FAKE-RATES //////////////////////

    DrawLHCCFakeRatePlot( 
        Plot<TProfile>("", LoadIDPVMHistogram<TProfile>(ttbarmu200, fake_vs_eta), "ITk, <#mu> = 200", "PL", PlotFormat().Color(kBlue).MarkerStyle(kFullCircle).MarkerSize(1.25)),
        Plot<TH1>("", LoadIDPVMEfficiency(run2_ttbar_fake, fake_vs_eta), "Run-2, <#mu> = 20", "L", PlotFormat().Color(kBlack).MarkerStyle(kOpenSquare).MarkerSize(1.25)),
        labels_ttbar, opts_ttbar_log, "|#eta|", "ttbar-fakerate_vs_eta");

    DrawLHCCPrimaryFakeRatePlot( 
        Plot<TEfficiency>("", LoadIDPVMHistogram<TEfficiency>(ttbarmu200, fake_primary_vs_eta), "ITk, <#mu> = 200", "PL", PlotFormat().Color(kBlue).MarkerStyle(kFullCircle).MarkerSize(1.25)),
        Plot<TEfficiency>("", LoadIDPVMHistogram<TEfficiency>(run2_ttbar_fake, fake_primary_vs_eta), "Run-2, <#mu> = 20", "L", PlotFormat().Color(kBlack).MarkerStyle(kOpenSquare).MarkerSize(1.25)),
        labels_ttbar, opts_ttbar_log, "|#eta|", "ttbar-primaryfakerate_vs_eta");

    // TCanvas * can = new TCanvas();
    // can->SetLogy();
    // Plot<TEfficiency> FR("", LoadIDPVMHistogram<TEfficiency>(run2_ttbar_fake, fake_primary_vs_eta), "Run-2", "L", PlotFormat().Color(kBlack).MarkerStyle(kOpenSquare).MarkerSize(1.25));
    // FR.applyFormat();
    // FR->Draw();
    // gPad->Update(); 
    // FR->GetPaintedGraph()->GetYaxis()->SetRangeUser(1.e-7, 1.e-1);
    // gPad->Update(); 
    // PlotUtils::saveCanvas(can, "CheckFR");

    return EXIT_SUCCESS;
}
