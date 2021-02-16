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

std::pair<std::vector<float>, std::vector<float>> getVectorsFromTree(const std::string & fname, const std::string & tname, const char* xbranch, const char* ybranch) {
    std::unique_ptr<TFile> myFile = std::make_unique<TFile>(fname.c_str(), "READ"); 
    
    TTree* myTree = nullptr;
    myFile->GetObject(tname.c_str(), myTree);
    if (!myTree) {
        std::cerr << "Failed to read tree" << std::endl;
        return {};
    }

    float xval; 
    float yval; 
    myTree->SetBranchAddress(xbranch, &xval);
    myTree->SetBranchAddress(ybranch, &yval);

    std::map<float, float> xy;
    for (Long64_t entry = 0; myTree->GetEntry(entry); ++entry){
        xy.emplace(xval, yval);
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
        for (size_t j = 0; j < r22.first.size(); ++j) {
            if (r21.first.at(i) == r22.first.at(j)) {
                x.push_back(r21.first.at(i));
                y.push_back(r22.second.at(j) / r21.second.at(i));
                break;
            }
        }
    }
    return std::make_pair(x, y);
}

void plotDiskVersusMu(Plot<TGraph> & graphR21, Plot<TGraph> & graphR22, Plot<TGraph> & graphR22NoLRT, Plot<TGraph> & graphRatio, Plot<TGraph> & graphRatioNoLRT) {
    graphR21.populate();
    graphR22.populate();
    graphR22NoLRT.populate();
    graphRatio.populate();
    graphRatioNoLRT.populate();

    CanvasOptions canvasOpts = CanvasOptions();

    // Get our colors
    Int_t colR21 = TColor::GetFreeColorIndex();
    TColor* c1 = new TColor(colR21, 0.42,0.70,0.32);

    Int_t colR22Full  = TColor::GetFreeColorIndex();
    TColor* c2 = new TColor(colR22Full, 0.12,0.47,0.71);

    Int_t colR22NoLRT = TColor::GetFreeColorIndex();
    TColor* c3 = new TColor(colR22NoLRT, 0.65,0.22,0.83);
    //

    PlotUtils::MultiPadCanvas mpc = canvasOpts.prepareMultiPadCanvas();

    // Upper pad plots
    mpc.getPad(0)->cd();
    graphR21->Draw("APL");
    graphR21->SetMarkerStyle(kFullSquare);
    graphR21->SetMarkerColor(colR21);
    graphR21->SetLineColor(colR21);
    graphR21->SetLineWidth(2);
    
    graphR22->Draw("SAMEPL");
    graphR22->SetMarkerStyle(kFullDotLarge);
    graphR22->SetMarkerColor(colR22Full);
    graphR22->SetLineColor(colR22Full);
    graphR22->SetLineWidth(2);

    graphR22NoLRT->Draw("SAMEPL");
    graphR22NoLRT->SetMarkerStyle(kOpenCircle);
    graphR22NoLRT->SetMarkerColor(colR22NoLRT);
    graphR22NoLRT->SetLineColor(colR22NoLRT);
    graphR22NoLRT->SetLineWidth(2);

    graphR21->GetXaxis()->SetLimits(9, 92);
    graphR21->GetYaxis()->SetRangeUser(-15, 500);

    // Lower pad ratio
    mpc.getPad(1)->cd();
    graphRatio->Draw("APL");
    graphRatio->SetMarkerStyle(kFullDotLarge);
    graphRatio->SetMarkerColor(colR22Full);
    graphRatio->SetLineColor(colR22Full);
    graphRatio->SetLineWidth(2);

    graphRatioNoLRT->Draw("SAMEPL");
    graphRatioNoLRT->SetMarkerStyle(kOpenCircle);
    graphRatioNoLRT->SetMarkerColor(colR22NoLRT);
    graphRatioNoLRT->SetLineColor(colR22NoLRT);
    graphRatioNoLRT->SetLineWidth(2);

    graphRatio->GetXaxis()->SetLimits(9, 92);
    graphRatio->GetYaxis()->SetRangeUser(0.195, 0.935);

    mpc.getCanvas()->Update();

    // Upper pad labels
    mpc.getPad(0)->cd();    
    canvasOpts = canvasOpts.doAtlasLabel(true).doLumiLabel(false).fontSize(24).atlasLabelX(0.04).atlasLabelY(-0.09);
    canvasOpts.drawLabels({});

    canvasOpts = canvasOpts.doAtlasLabel(false).doLumiLabel(false).fontSize(22).otherLabelX(0.04).otherLabelStartY(-0.16);
    canvasOpts.drawLabels({"#it{AOD} Disk Space (ID-only)", "Run-2 Data Reconstruction"});

    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{
        PlotUtils::LegendEntry(graphR21(),      "Run-2 Reconstruction",         "PL"),
        PlotUtils::LegendEntry(graphR22(),      "Run-3 Reconstruction",         "PL"),
        PlotUtils::LegendEntry(graphR22NoLRT(), "Run-3 Reconstruction w/o LRT", "PL"),
    },
        0.20, 0.45, 0.55, 0.65
    );
    
    PlotUtils::drawTLatex(0.795, 0.84, "Not on GRL", 18, 53, 11, kGray+3);
    PlotUtils::drawLine(71, -15, 71, 500, kBlack, kDashed, 3);
    TBox box1(71, -15, 92, 500);
    box1.SetFillColor(kGray+2);
    box1.SetFillStyle(3004);
    box1.Draw();

    graphR21->GetYaxis()->SetTitle("Disk Space [kb/event]");
    graphR21->GetXaxis()->SetTitleFont(43);
    graphR21->GetYaxis()->SetTitleFont(43);
    graphR21->GetXaxis()->SetLabelFont(43);
    graphR21->GetYaxis()->SetLabelFont(43);
    graphR21->GetXaxis()->SetTitleSize(24); 
    graphR21->GetYaxis()->SetTitleSize(24); 
    graphR21->GetXaxis()->SetLabelSize(22); 
    graphR21->GetYaxis()->SetLabelSize(22); 
    
    graphR21->GetYaxis()->SetTitleOffset(1.05);

    // Lower pad labels
    mpc.getPad(1)->cd();
    PlotUtils::drawLine(71, 0.195, 71, 0.935, kBlack, kDashed, 3);
    TBox box2(71, 0.195, 92, 0.935);
    box2.SetFillColor(kGray+2);
    box2.SetFillStyle(3004);
    box2.Draw();
    
    graphRatio->GetXaxis()->SetTitle("< #mu >");
    graphRatio->GetYaxis()->SetTitle("Run-3 / Run-2");
    graphRatio->GetXaxis()->SetTitleFont(43);
    graphRatio->GetYaxis()->SetTitleFont(43);
    graphRatio->GetXaxis()->SetLabelFont(43);
    graphRatio->GetYaxis()->SetLabelFont(43);
    graphRatio->GetXaxis()->SetTitleSize(24); 
    graphRatio->GetYaxis()->SetTitleSize(24); 
    graphRatio->GetXaxis()->SetLabelSize(22); 
    graphRatio->GetYaxis()->SetLabelSize(22); 

    graphRatio->GetXaxis()->SetTitleOffset(2.25);
    graphRatio->GetYaxis()->SetTitleOffset(1.05);

    canvasOpts.SaveCanvas(mpc.getCanvas(), "Size_vs_mu_run2_vs_run3");
}

int main (int, char**) {

    const std::string tree  = "Squirrels";
    const std::string inR21 = "/home/zschilla/Working/IDPVMAnalysis/run/SPOT/v02/SPOTResults.r21-2021-02-16.root";        
    const std::string inR22 = "/home/zschilla/Working/IDPVMAnalysis/run/SPOT/v02/SPOTResults.r22-2021-02-16.root";

    SetAtlasStyle();
    // Configure branch names
    const char* mu = "Mu";
    const char* disk = "TotalDisk";
    const char* LRT  = "DiskFromLRT";

    auto r21muDisk      = getVectorsFromTree(inR21, tree, mu, disk);
    auto r22muDisk      = getVectorsFromTree(inR22, tree, mu, disk);
    auto r22muDiskNoLRT = getVectorsFromTree(inR22, tree, mu, LRT);
    for (size_t i = 0; (i < r22muDisk.first.size() && i < r22muDiskNoLRT.first.size()); ++i) {
        if (r22muDisk.first.at(i) == r22muDiskNoLRT.first.at(i)) {
            r22muDiskNoLRT.second.at(i) = r22muDisk.second.at(i) - r22muDiskNoLRT.second.at(i);
        }
    }

    auto ratio      = getRatio(r21muDisk, r22muDisk);
    auto ratioNoLRT = getRatio(r21muDisk, r22muDiskNoLRT);

    Plot<TGraph> graphR21("graphR21", std::make_shared<TGraph>(r21muDisk.first.size(), &(r21muDisk.first[0]), &(r21muDisk.second[0])).get());
    Plot<TGraph> graphR22("graphR22", std::make_shared<TGraph>(r22muDisk.first.size(), &(r22muDisk.first[0]), &(r22muDisk.second[0])).get());
    Plot<TGraph> graphR22NoLRT("graphR22NoLRT", std::make_shared<TGraph>(r22muDiskNoLRT.first.size(), &(r22muDiskNoLRT.first[0]), &(r22muDiskNoLRT.second[0])).get());

    Plot<TGraph> graphRatio("graphRatio", std::make_shared<TGraph>(ratio.first.size(), &(ratio.first[0]), &(ratio.second[0])).get());
    Plot<TGraph> graphRatioNoLRT("graphRatioNoLRT", std::make_shared<TGraph>(ratioNoLRT.first.size(), &(ratioNoLRT.first[0]), &(ratioNoLRT.second[0])).get());

    plotDiskVersusMu(graphR21, graphR22, graphR22NoLRT, graphRatio, graphRatioNoLRT);

    return EXIT_SUCCESS;
}
