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

void DrawDynamicLabels(const std::vector<std::string> & labels, double xstart = 0.10, double ystart = 0.90, double ystep = 0.06) {
    PlotUtils::drawAtlas(xstart, ystart, "Simulation Preliminary");
    ystart -= ystep;
    PlotUtils::drawLumiSqrtS(xstart, ystart, "HL-LHC", "14 TeV");
    for (auto & label : labels) {
        int fontsize = (label.find("ATLAS-P2") == std::string::npos) ? 18   : 16;
        // ystart      -= (label.find("ATLAS-P2") == std::string::npos) ? 0.07 : 0.05;
        ystart -= ystep;
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
template <class H> void FormatITk(H & plot, const std::string & filename, int color=kBlue, int markerstyle=kOpenCircle) {
    PlotFormat formatITk  = PlotFormat().Color(color).MarkerStyle(markerstyle).MarkerSize(1.25).ExtraDrawOpts("HISTE1");
    plot.setPlotFormat(formatITk);
    plot.applyFormat();

    // Restrict eta within {-4., 4.}
    if (filename.find("_eta") != std::string::npos) {
        std::cout << "Restricting plot " << plot.getName() << " between -4.0 and 4.0 in eta!" << std::endl;
        for (int i = 1; i < plot->GetNbinsX()+1; ++i) {
            if (fabs(plot->GetBinCenter(i)) > 4.0) {
                std::cout << "Forcing bin " << plot->GetBinLowEdge(i) << "-" << plot->GetBinLowEdge(i+1) << " to zero" << std::endl;
                plot->SetBinContent(i, 0.);
                plot->SetBinError(i, 0.);
            }
        }
    }
}
template <class H> void FormatRun2(H & plot, const std::string & filename) {
    PlotFormat formatRun2 = PlotFormat().Color(kBlack).LineStyle(kDashed).LineWidth(3.5).ExtraDrawOpts("HIST][");
    plot.setPlotFormat(formatRun2);
    plot.applyFormat();

    // Restrict eta within {-2.4, 2.4}
    if (filename.find("_eta") != std::string::npos) {
        std::cout << "Restricting plot " << plot.getName() << " between -2.4 and 2.4 in eta!" << std::endl;
        for (int i = 1; i < plot->GetNbinsX()+1; ++i) {
            if (fabs(plot->GetBinCenter(i)) > 2.4) {
                std::cout << "Forcing bin " << plot->GetBinLowEdge(i) << "-" << plot->GetBinLowEdge(i+1) << " to zero" << std::endl;
                plot->SetBinContent(i, 0.);
                plot->SetBinError(i, 0.);
            }
        }
    }
}

void DrawPubNoteResolutionPlots(const Plot<TH1F> & h_ITk, const Plot<TH1F> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename, const std::string & multiPagePdf="", std::pair<double, double> minmax={0,0}) {
    SetAtlasStyle();
    
    Plot<TH1F> itk("h_ITk", h_ITk);
    FormatITk(itk, filename);
    
    Plot<TH1F> run2("h_Run2", h_Run2);
    FormatRun2(run2, filename);
    FormatAxisLabelSize(run2);

    Plot<TH1> ratio("ratio", PlotUtils::getRatio(itk(), run2(), PlotUtils::EfficiencyMode::defaultErrors));
    FormatITk(ratio, filename);
    FormatAxisLabelSize(ratio, true);
    ratio->GetXaxis()->SetTitle(xlabel.c_str());
    ratio->GetYaxis()->SetTitle("ITk / Run-2");

    auto mpc = PlotUtils::prepareTwoPadCanvas("", 0.4, canvasOpts.canSizeX(), canvasOpts.canSizeY());
    
    mpc.getPad(0)->cd();
    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();
    
    if (minmax.first != minmax.second) {
        run2->SetMinimum(minmax.first);
        run2->SetMaximum(minmax.second);
    }

    run2->Draw(std::string("" + (run2.plotFormat().ExtraDrawOpts().isSet ? run2.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    itk->Draw(std::string("SAME" + (itk.plotFormat().ExtraDrawOpts().isSet ? itk.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    gPad->RedrawAxis();

    DrawDynamicLabels(labels, 0.15, 0.88);
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{PlotUtils::LegendEntry(run2), PlotUtils::LegendEntry(itk)}, 0.60,0.76,0.88,0.92);

    mpc.getPad(1)->cd();
    if (canvasOpts.logX())     gPad->SetLogx();
    if (canvasOpts.logRatio()) gPad->SetLogy();
    
    ratio->SetMinimum(0.0);
    ratio->SetMaximum(1.49);

    ratio->Draw("HIST][");
    ratio->Draw("SAMEE1");
    gPad->RedrawAxis();

    canvasOpts.SaveCanvas(mpc.getCanvas(), filename, multiPagePdf);
}

void DrawPubNoteFakeRatePlot(const Plot<TProfile> & h_ITk, const Plot<TProfile> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename, const std::string & multiPagePdf="", std::pair<double, double> minmax={0,0}) {
    SetAtlasStyle();

    Plot<TProfile> itk("h_ITk", h_ITk);
    FormatITk(itk, filename);
    
    Plot<TProfile> run2("h_Run2", h_Run2);
    FormatRun2(run2, filename);
    FormatAxisLabelSize(run2);
    run2->GetXaxis()->SetTitle(xlabel.c_str());
    run2->GetYaxis()->SetTitle("Fake Rate");

    std::shared_ptr<TCanvas> can = std::make_shared<TCanvas>("can","can",canvasOpts.canSizeX(),canvasOpts.canSizeY());
    
    can->cd();
    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();

    if (minmax.first != minmax.second) {
        run2->SetMinimum(minmax.first);
        run2->SetMaximum(minmax.second);
    }
    
    run2->Draw(std::string("" + (run2.plotFormat().ExtraDrawOpts().isSet ? run2.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    itk->Draw(std::string("SAME" + (itk.plotFormat().ExtraDrawOpts().isSet ? itk.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    gPad->RedrawAxis();

    DrawDynamicLabels(labels, 0.20, 0.90, 0.04 );
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{PlotUtils::LegendEntry(run2), PlotUtils::LegendEntry(itk)}, 0.60,0.76,0.88,0.92);
    
    canvasOpts.SaveCanvas(can, filename, multiPagePdf);
}

void DrawPubNotePrimaryFakeRatePlot(const Plot<TH1> & h_ITk, const Plot<TH1> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename, const std::string & multiPagePdf="", std::pair<double, double> minmax={0,0}) {
    SetAtlasStyle();

    Plot<TH1> itk("h_ITk", h_ITk);
    FormatITk(itk, filename);
    
    Plot<TH1> run2("h_Run2", h_Run2);
    FormatRun2(run2, filename);
    FormatAxisLabelSize(run2);
    run2->GetXaxis()->SetTitle(xlabel.c_str());
    run2->GetYaxis()->SetTitle("Primary Fake Rate");

    std::shared_ptr<TCanvas> can = std::make_shared<TCanvas>("can","can",canvasOpts.canSizeX(),canvasOpts.canSizeY());
    
    can->cd();
    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();

    if (minmax.first != minmax.second) {
        run2->SetMinimum(minmax.first);
        run2->SetMaximum(minmax.second);
    }

    run2->Draw(std::string("" + (run2.plotFormat().ExtraDrawOpts().isSet ? run2.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    itk->Draw(std::string("SAME" + (itk.plotFormat().ExtraDrawOpts().isSet ? itk.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    gPad->RedrawAxis();

    DrawDynamicLabels(labels, 0.20, 0.90, 0.04 );
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{PlotUtils::LegendEntry(run2), PlotUtils::LegendEntry(itk)}, 0.60,0.76,0.88,0.92);
    
    canvasOpts.SaveCanvas(can, filename, multiPagePdf);
}

void DrawPubNoteEfficiencyPlot(const Plot<TH1> & h_ITk, const Plot<TH1> & h_Run2, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename, const std::string & multiPagePdf="", std::pair<double, double> minmax={0,0}) {
    SetAtlasStyle();

    Plot<TH1> itk("h_ITk", h_ITk);
    FormatITk(itk, filename);
    
    Plot<TH1> run2("h_Run2", h_Run2);
    FormatRun2(run2, filename);
    FormatAxisLabelSize(run2);

    Plot<TH1> ratio("ratio", PlotUtils::getRatio(itk(), run2(), PlotUtils::EfficiencyMode::defaultErrors));
    FormatITk(ratio, filename);
    FormatAxisLabelSize(ratio, true);
    ratio->GetXaxis()->SetTitle(xlabel.c_str());
    ratio->GetYaxis()->SetTitle("ITk / Run-2");

    auto mpc = PlotUtils::prepareTwoPadCanvas("", 0.4, canvasOpts.canSizeX(), canvasOpts.canSizeY());

    mpc.getPad(0)->cd();
    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();

    if (minmax.first != minmax.second) {
        run2->SetMinimum(minmax.first);
        run2->SetMaximum(minmax.second);
    }
    run2->GetYaxis()->SetNdivisions(8,5,0);

    run2->Draw(std::string("" + (run2.plotFormat().ExtraDrawOpts().isSet ? run2.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    itk->Draw(std::string("SAME" + (itk.plotFormat().ExtraDrawOpts().isSet ? itk.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    gPad->RedrawAxis();

    DrawDynamicLabels(labels, 0.15, 0.88);
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{PlotUtils::LegendEntry(run2), PlotUtils::LegendEntry(itk)}, 0.60,0.79,0.88,0.95);

    mpc.getPad(1)->cd();
    if (canvasOpts.logX())     gPad->SetLogx();
    if (canvasOpts.logRatio()) gPad->SetLogy();
    
    ratio->SetMinimum(0.915);
    ratio->SetMaximum(1.065);
    ratio->GetYaxis()->SetNdivisions(3,5,0);

    ratio->Draw("HIST][");
    ratio->Draw("SAMEE1");
    gPad->RedrawAxis();

    PlotUtils::saveCanvas(mpc.getCanvas(), filename);
    PlotUtils::saveCanvasToMultiPagePdfFile(mpc.getCanvas(), multiPagePdf);
}

void DrawPubNoteSingleParticleEfficiencyPlot(const Plot<TH1> & h_EffPi, const Plot<TH1> & h_EffEl, const Plot<TH1> & h_EffMu, const std::vector<std::string> & labels, CanvasOptions & canvasOpts, const std::string & xlabel, const std::string & filename, const std::string & multiPagePdf="", std::pair<double, double> minmax={0,0}) {
    SetAtlasStyle();

    Plot<TH1> effPi("h_EffPi", h_EffPi);
    FormatITk(effPi, filename, kGreen+3, kFullCross);
    
    Plot<TH1> effEl("h_EffEl", h_EffEl);
    FormatITk(effEl, filename, kRed, kFullSquare);
    
    Plot<TH1> effMu("h_EffMu", h_EffMu);
    FormatITk(effMu, filename, kBlue, kFullCircle);
    FormatAxisLabelSize(effMu);
    effMu->GetXaxis()->SetTitle(xlabel.c_str());

    std::shared_ptr<TCanvas> can = std::make_shared<TCanvas>("can","can",canvasOpts.canSizeX(),canvasOpts.canSizeY());
    
    can->cd();
    if (canvasOpts.logX()) gPad->SetLogx();
    if (canvasOpts.logY()) gPad->SetLogy();

    if (minmax.first != minmax.second) {
        effMu->SetMinimum(minmax.first);
        effMu->SetMaximum(minmax.second);
    }

    effMu->Draw(std::string("" + (effMu.plotFormat().ExtraDrawOpts().isSet ? effMu.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    effEl->Draw(std::string("SAME" + (effEl.plotFormat().ExtraDrawOpts().isSet ? effEl.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    effPi->Draw(std::string("SAME" + (effPi.plotFormat().ExtraDrawOpts().isSet ? effPi.plotFormat().ExtraDrawOpts().val : std::string(""))).c_str());
    gPad->RedrawAxis();

    DrawDynamicLabels(labels, 0.20, 0.90, 0.04 );
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{PlotUtils::LegendEntry(effMu), PlotUtils::LegendEntry(effEl), PlotUtils::LegendEntry(effPi)}, 0.60,0.75,0.88,0.92);

    PlotUtils::saveCanvas(can, filename);
    PlotUtils::saveCanvasToMultiPagePdfFile(can, multiPagePdf);
}

int main (int, char**) {

    // ATLAS-P2-ITK-23-00-01
    const std::string sglmu1_01   = "/scratch/Datasets/ANA-IDTR-2020-01/ATLAS-P2-ITK-23-00-01/sglmu1_PU0_IDPVM_r12350.root";
    const std::string sglmu100_01 = "/scratch/Datasets/ANA-IDTR-2020-01/ATLAS-P2-ITK-23-00-01/sglmu100_PU0_IDPVM_r12350.root";
    const std::string ttbarmu200  = "/scratch/Datasets/ANA-IDTR-2020-01/ATLAS-P2-ITK-23-00-01/ttbar_PU200_IDPVM_r12351.root";

    // ATLAS-P2-ITK-23-00-03
    const std::string sglmu1     = "/scratch/Datasets/ANA-IDTR-2020-01/ATLAS-P2-ITK-23-00-03/sglmu1_PU0_IDPVM_r12440.root";
    const std::string sglmu10    = "/scratch/Datasets/ANA-IDTR-2020-01/ATLAS-P2-ITK-23-00-03/sglmu10_PU0_IDPVM_r12440.root";
    const std::string sglmu100   = "/scratch/Datasets/ANA-IDTR-2020-01/ATLAS-P2-ITK-23-00-03/sglmu100_PU0_IDPVM_r12440.root";

    const std::string sglel10    = "/scratch/Datasets/ANA-IDTR-2020-01/ATLAS-P2-ITK-23-00-03/sglel10_PU0_IDPVM_r12440.root";
    const std::string sglpi10    = "/scratch/Datasets/ANA-IDTR-2020-01/ATLAS-P2-ITK-23-00-03/sglpi10_PU0_IDPVM_r12440.root";

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
    // Primary Fakes - TEfficiency
    const std::string fake_primary_vs_eta = "IDPerformanceMon/Tracks/SelectedFakeTracks/fakeetaPrimary";
    const std::string fake_primary_vs_pt  = "IDPerformanceMon/Tracks/SelectedFakeTracks/fakeptPrimary";
    // Fakes - TProfile
    const std::string fake_vs_eta         = "IDPerformanceMon/Tracks/SelectedFakeTracks/track_fakerate_vs_eta";
    const std::string fake_vs_pt          = "IDPerformanceMon/Tracks/SelectedFakeTracks/track_fakerate_vs_pt";

    // Labels
    const std::vector<std::string> labels_muons_pt1   = {"ITk Layout: ATLAS-P2-ITK-23-00-03", "Single #mu, p_{T} = 1 GeV"}; 
    const std::vector<std::string> labels_muons_pt10  = {"ITk Layout: ATLAS-P2-ITK-23-00-03", "Single #mu, p_{T} = 10 GeV"}; 
    const std::vector<std::string> labels_muons_pt100 = {"ITk Layout: ATLAS-P2-ITK-23-00-03", "Single #mu, p_{T} = 100 GeV"}; 
    const std::vector<std::string> labels_single_pt10 = {"ITk Layout: ATLAS-P2-ITK-23-00-03", "Single Particle, p_{T} = 10 GeV"}; 
    const std::vector<std::string> labels_ttbar_pt    = {"ITk Layout: ATLAS-P2-ITK-23-00-01", "t#bar{t}, p_{T} > 1 GeV"}; 
    const std::vector<std::string> labels_ttbar       = {"ITk Layout: ATLAS-P2-ITK-23-00-01", "t#bar{t}"}; 
    
    // CanvasOptions
    CanvasOptions opts      = CanvasOptions();
    CanvasOptions opts_logY = CanvasOptions(opts).logY(true);

    SetAtlasStyle();
    
    const std::string multiPagePdf = "ITkPubNotePlots";
    PlotUtils::startMultiPagePdfFile(multiPagePdf);

    ////////////////////// RESOLUTION //////////////////////
    
    // d0
    DrawPubNoteResolutionPlots( 
        Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu1,      d0_vs_eta), "ITk",  "PL"),
        Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(run2_sglmu1, d0_vs_eta), "Run-2", "L"),
        labels_muons_pt1, opts_logY, "Truth #eta", "SingleMu1-d0Resolution_vs_eta", multiPagePdf, {7.e0, 5.e3});
    
    DrawPubNoteResolutionPlots( 
        Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu100,      d0_vs_eta), "ITk",  "PL"),
        Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(run2_sglmu100, d0_vs_eta), "Run-2", "L"),
        labels_muons_pt100, opts_logY, "Truth #eta", "SingleMu100-d0Resolution_vs_eta", multiPagePdf, {7.e-1, 1.e3});

    // z0
    DrawPubNoteResolutionPlots( 
        Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu1,      z0_vs_eta), "ITk",  "PL"),
        Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(run2_sglmu1, z0_vs_eta), "Run-2", "L"),
        labels_muons_pt1, opts_logY, "Truth #eta", "SingleMu1-z0Resolution_vs_eta", multiPagePdf, {7.e0, 5.e5});

    DrawPubNoteResolutionPlots( 
        Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(sglmu100,      z0_vs_eta), "ITk",  "PL"),
        Plot<TH1F>("", LoadIDPVMHistogram<TH1F>(run2_sglmu100, z0_vs_eta), "Run-2", "L"),
        labels_muons_pt100, opts_logY, "Truth #eta", "SingleMu100-z0Resolution_vs_eta", multiPagePdf, {7.e-1, 1.e4});

    ////////////////////// EFFICIENCY //////////////////////

    // muons
    DrawPubNoteEfficiencyPlot( 
        Plot<TH1>("", LoadIDPVMEfficiency(sglmu1,      eff_vs_eta), "ITk", "PL"),
        Plot<TH1>("", LoadIDPVMEfficiency(run2_sglmu1, eff_vs_eta), "Run-2", "L"),
        labels_muons_pt1, opts, "Truth #eta", "SingleMu1-efficiency_vs_eta", multiPagePdf, {0.967, 1.025});
    
    DrawPubNoteEfficiencyPlot( 
        Plot<TH1>("", LoadIDPVMEfficiency(sglmu100,      eff_vs_eta), "ITk", "PL"),
        Plot<TH1>("", LoadIDPVMEfficiency(run2_sglmu100, eff_vs_eta), "Run-2", "L"),
        labels_muons_pt100, opts, "Truth #eta", "SingleMu100-efficiency_vs_eta", multiPagePdf, {0.989, 1.009});

    // single particles (ITk only)
    DrawPubNoteSingleParticleEfficiencyPlot( 
        Plot<TH1>("", LoadIDPVMEfficiency(sglpi10, eff_vs_eta), "Single #pi", "PL"),
        Plot<TH1>("", LoadIDPVMEfficiency(sglel10, eff_vs_eta), "Single e",   "PL"),
        Plot<TH1>("", LoadIDPVMEfficiency(sglmu10, eff_vs_eta), "Single #mu", "PL"),
        labels_single_pt10, opts, "Truth #eta", "SingleParticle10-efficiency_vs_eta", multiPagePdf, {0.825, 1.095});

    // ttbar
    DrawPubNoteEfficiencyPlot( 
        Plot<TH1>("", LoadIDPVMEfficiency(ttbarmu200,     eff_vs_eta), "ITk, #LT#mu#GT = 200", "PL"),
        Plot<TH1>("", LoadIDPVMEfficiency(run2_ttbarmu20, eff_vs_eta), "Run-2, #LT#mu#GT = 20", "L"),
        labels_ttbar_pt, opts, "Truth #eta", "ttbar-efficiency_vs_eta", multiPagePdf, {0.64, 1.014});

    DrawPubNoteEfficiencyPlot( 
        Plot<TH1>("", LoadIDPVMEfficiency(ttbarmu200,     eff_vs_pt), "ITk |#eta| < 4.0, #LT#mu#GT = 200", "PL"),
        Plot<TH1>("", LoadIDPVMEfficiency(run2_ttbarmu20, eff_vs_pt), "Run-2 |#eta| < 2.4, #LT#mu#GT = 20", "L"),
        labels_ttbar, opts, "Truth p_{T} [GeV]", "ttbar-efficiency_vs_pt", multiPagePdf, {0.78, 1.015});

    ////////////////////// FAKE-RATES //////////////////////

    DrawPubNoteFakeRatePlot( 
        Plot<TProfile>("", LoadIDPVMHistogram<TProfile>(ttbarmu200,     fake_vs_eta), "ITk, #LT#mu#GT = 200", "PL"),
        Plot<TProfile>("", LoadIDPVMHistogram<TProfile>(run2_ttbarmu20, fake_vs_eta), "Run-2, #LT#mu#GT = 20", "L"),
        labels_ttbar, opts_logY, "Track #eta", "ttbar-fakerate_vs_eta", multiPagePdf, {1.e-7, 1.5e0});

    DrawPubNoteFakeRatePlot( 
        Plot<TProfile>("", LoadIDPVMHistogram<TProfile>(ttbarmu200,     fake_vs_pt), "ITk, #LT#mu#GT = 200", "PL"),
        Plot<TProfile>("", LoadIDPVMHistogram<TProfile>(run2_ttbarmu20, fake_vs_pt), "Run-2, #LT#mu#GT = 20", "L"),
        labels_ttbar, opts_logY, "Track p_{T} [GeV]", "ttbar-fakerate_vs_pt", multiPagePdf, {1.e-7, 1.5e0});

    DrawPubNotePrimaryFakeRatePlot( 
        Plot<TH1>("", LoadIDPVMEfficiency(ttbarmu200,     fake_primary_vs_eta), "ITk, #LT#mu#GT = 200", "PL"),
        Plot<TH1>("", LoadIDPVMEfficiency(run2_ttbarmu20, fake_primary_vs_eta), "Run-2, #LT#mu#GT = 20", "L"),
        labels_ttbar, opts_logY, "Track #eta", "ttbar-fakerateprimary_vs_eta", multiPagePdf, {1.e-6, 5.e-1});

    PlotUtils::endMultiPagePdfFile(multiPagePdf);

    return EXIT_SUCCESS;
}
