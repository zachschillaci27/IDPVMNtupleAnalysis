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

class IDSoftwareTree : public NtupleBranchMgr{

    public: 
        IDSoftwareTree(TTree* t): 
            NtupleBranchMgr(t), 
            year("Year",t,this),
            mu("Mu",t,this),
            disk("Disk",t,this) {
                getMissedBranches(t);
        }
        NtupleBranch<int>   year;
        NtupleBranch<float> mu;
        NtupleBranch<float> disk;
};

void normalizeByEvents(Plot<TH1D> & plot) {
    plot.populate();

    const std::vector<std::pair<float, int>> nEventsByMu {
        {16,	2969},
        {20,	2749},
        {25,	2486},
        {30,	2301},
        {35,	2132},
    //
        {38,	2469},
        {40,	2416},
        {45,	2284},
        {50,	2169},
        {55,	2093},
        {58,	2047},
    // 
        {17.3,  3053},
        {20,	2922},
        {25,	2726},
        {30,	2557},
        {35,	2410},
        {40,	2290},
        {45,	2181},
        {48,	1909},
    // 
        {52,	2045},
        {54.7,	1961},
    };

    std::cout << "--- normalizeByEvents ---" << std::endl;
    for (int i = 1; i < plot->GetXaxis()->GetNbins() + 1; ++i) {
        int nEvents = 0;
        for (auto & entry : nEventsByMu) {
            int nbin = plot->FindBin(entry.first);
            if (i == nbin) {
                nEvents += entry.second;
            }
        }
        std::cout << "---> Pileup: " << plot->GetXaxis()->GetBinLowEdge(i) << " - " << plot->GetXaxis()->GetBinUpEdge(i) << std::endl;
        std::cout << "---> nEvents: " << nEvents << std::endl;
        plot->SetBinContent(i, (1 / nEvents) * plot->GetBinContent(i));
    }
}

std::pair<std::vector<float>, std::vector<float>> getDiskVersusMu(const std::string & fname, const std::string & tname) {
    std::unique_ptr<TFile> myFile = std::make_unique<TFile>(fname.c_str(), "READ"); 
    
    TTree* myTree = nullptr;
    myFile->GetObject(tname.c_str(), myTree);
    if (!myTree) {
        std::cerr << "Failed to read tree" << std::endl;
        return {};
    }

    float mu; 
    float disk; 
    myTree->SetBranchAddress("Mu", &mu);
    myTree->SetBranchAddress("Disk", &disk);

    std::map<float, float> xy;
    for (Long64_t entry = 0; myTree->GetEntry(entry); ++entry){
        xy.emplace(mu, disk);
    }
    myFile->Close();
    
    std::cout << fname << std::endl;
    std::vector<float> x;
    std::vector<float> y;
    for (auto & entry : xy) {
        std::cout << entry.first << ", " << entry.second << std::endl;
        x.push_back(entry.first);
        y.push_back(entry.second);
    }

    return std::make_pair(x, y);
}

std::pair<std::vector<float>, std::vector<float>> getRatio(std::pair<std::vector<float>, std::vector<float>> & r21, std::pair<std::vector<float>, std::vector<float>> & r22) {
    std::vector<float> x;
    std::vector<float> y;
    for (size_t i = 0; i < r21.first.size(); ++i) {
        if (r21.first.at(i) == r22.first.at(i)) {
            x.push_back(r21.first.at(i));
            y.push_back(r22.second.at(i) / r21.second.at(i));
        }
    }
    return std::make_pair(x, y);
}

void plotDiskVersusMu(Plot<TGraph> & r21, Plot<TGraph> & r22, Plot<TGraph> & ratio) {
    r21.populate();
    r22.populate();
    ratio.populate();
    
    CanvasOptions canvasOpts = CanvasOptions().doLumiLabel(false);

    PlotUtils::MultiPadCanvas mpc = canvasOpts.prepareMultiPadCanvas();
    mpc.getPad(0)->cd();

    r21->Draw("AL*");
    r21->SetMarkerStyle(r21.plotFormat().getMarkerStyle().value());
    r21->GetXaxis()->SetLimits(14.5, 60.5);
    r21->GetYaxis()->SetRangeUser(900, 3500);
    r21->GetYaxis()->SetTitle("Disk Space [kb/event]");
    r21->GetYaxis()->SetTitleSize(0.065);
    r21->GetYaxis()->SetTitleOffset(0.75);

    r22->Draw("SAMEL*");
    r22->SetMarkerStyle(r22.plotFormat().getMarkerStyle().value());

    mpc.getPad(1)->cd();
    
    ratio->Draw("AL*");
    ratio->SetMarkerStyle(ratio.plotFormat().getMarkerStyle().value());
    ratio->GetXaxis()->SetLimits(14.5, 60.5);
    ratio->GetXaxis()->SetTitle("<#mu>");
    // ratio->GetXaxis()->SetTitle("Year [A.D.]");
    // ratio->GetXaxis()->SetLimits(2014.75, 2018.25);
    // ratio->GetXaxis()->SetNdivisions(4);
    ratio->GetYaxis()->SetTitle(ratio.getLegendTitle().c_str());
    ratio->GetXaxis()->SetTitleSize(0.075);
    ratio->GetXaxis()->SetLabelSize(0.075);
    ratio->GetYaxis()->SetTitleSize(0.075);
    ratio->GetYaxis()->SetLabelSize(0.065);
    ratio->GetYaxis()->SetTitleOffset(0.55);

    mpc.getCanvas()->Update();

    mpc.getPad(0)->cd();
    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{
        PlotUtils::LegendEntry(r21(), r21.getLegendTitle(), r21.getLegendOption()),
        PlotUtils::LegendEntry(r22(), r22.getLegendTitle(), r22.getLegendOption()),
    },
        0.20, 0.45, 0.40, 0.65
    );
    canvasOpts.drawLabels({"Run-2 Data Reconstruction", "ESD Disk Space (#it{to be updated to AOD})"});

    canvasOpts.SaveCanvas(mpc.getCanvas(), "DiskSpaceVersusMu");
}

int main (int, char**) {

    const std::string tree  = "Squirrels";
    const std::string inR21 = "/home/zschilla/Working/IDPVMAnalysis/run/SPOTResults.rel21Test.root";        
    const std::string inR22 = "/home/zschilla/Working/IDPVMAnalysis/run/SPOTResults.rel22Test.root";

    SetAtlasStyle();

    // NTAU Histogram Approach

    // Sample<IDSoftwareTree> sampleR21("sampleR21", inR21, tree);
    // Sample<IDSoftwareTree> sampleR22("sampleR22", inR22, tree);

    // Selection<IDSoftwareTree> dummy("dummy", [](IDSoftwareTree &) { return true; });
   
    // TH1D mu("mu",";<#mu>;Disk Space [kb/event]",9,17.5,62.5);
    // PlotFillInstructionWithRef<TH1D, IDSoftwareTree> diskMuFiller("diskMuFiller", [](TH1D* h, IDSoftwareTree &t) 
    //     { 
    //         int nbin       = h->FindBin(t.mu());
    //         float content = h->GetBinContent(nbin);
    //         h->SetBinContent(nbin, content + t.disk());
    //     }, &mu);

    // TH1D year("year",";Year [A.D];Disk Space [kb/event]",4,2015,2019);
    // PlotFillInstructionWithRef<TH1D, IDSoftwareTree> diskYearFiller("diskYearFiller", [](TH1D* h, IDSoftwareTree &t) 
    //     { 
    //         int nbin       = h->FindBin(t.year());
    //         float content = h->GetBinContent(nbin);
    //         h->SetBinContent(nbin, content + t.disk());
    //     }, &year);

    // Plot<TH1D> plotMuR21(sampleR21, dummy, diskMuFiller, "Rel. 21", "L", PremadePlotFormats::Lines1().ExtraDrawOpts("HIST"));
    // Plot<TH1D> plotMuR22(sampleR22, dummy, diskMuFiller, "Rel. 22", "L", PremadePlotFormats::Lines2().ExtraDrawOpts("HIST"));

    // Plot<TH1D> plotYearR21(sampleR21, dummy, diskYearFiller, "Rel. 21", "L", PremadePlotFormats::Lines1().ExtraDrawOpts("HIST"));
    // Plot<TH1D> plotYearR22(sampleR22, dummy, diskYearFiller, "Rel. 22", "L", PremadePlotFormats::Lines2().ExtraDrawOpts("HIST"));

    // PlotContent<TH1D> muPlots({plotMuR21, plotMuR22}, {"Run-2 Data Reconstruction", "AOD Disk Space"}, "DiskSpaceVersusMu", "", 
    //     CanvasOptions().doLumiLabel(false).yMin(905).yMax(4250).ratioAxisTitle("Rel. 22 / Rel. 21").ratioMin(0.35).ratioMax(1.05).extraXtitleOffset(0.85));
    // DefaultPlotting::draw1DWithRatio(muPlots);

    // PlotContent<TH1D> yearPlots({plotYearR21, plotYearR22}, {"Run-2 Data Reconstruction", "AOD Disk Space"}, "DiskSpaceVersusYear", "", 
    //     CanvasOptions().doLumiLabel(false).yMin(905).yMax(4250).ratioAxisTitle("Rel. 22 / Rel. 21").ratioMin(0.0).ratioMax(1.05).extraXtitleOffset(0.85));
    // DefaultPlotting::draw1DWithRatio(yearPlots);

    // ROOT TGraph Approach ('old school')

    auto r21xy = getDiskVersusMu(inR21, tree);
    auto r22xy = getDiskVersusMu(inR22, tree);
    auto ratio = getRatio(r21xy, r22xy);

    std::cout << r21xy.first.size() <<  " - " << r22xy.first.size() << std::endl;

    Plot<TGraph> graphMuR21("graphMuR21", std::make_shared<TGraph>(r21xy.first.size(), &(r21xy.first[0]), &(r21xy.second[0])).get(), "Rel. 21", "PL", PremadePlotFormats::Markers1());
    Plot<TGraph> graphMuR22("graphMuR22", std::make_shared<TGraph>(r22xy.first.size(), &(r22xy.first[0]), &(r22xy.second[0])).get(), "Rel. 22", "PL", PremadePlotFormats::Markers2());
    Plot<TGraph> graphRatio("graphRatio", std::make_shared<TGraph>(ratio.first.size(), &(ratio.first[0]), &(ratio.second[0])).get(), "Rel. 22 / Rel. 21", "PL", PremadePlotFormats::Markers2());

    plotDiskVersusMu(graphMuR21, graphMuR22, graphRatio);

    return EXIT_SUCCESS;
}
