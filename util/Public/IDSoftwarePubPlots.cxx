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

TGraph* getRatio(const TGraph* num, const TGraph* den) {
    TGraph* theRatio = new TGraph; 
    for (int k = 0; k < num->GetN(); ++k){
        if (std::abs(num->GetX()[k] - den->GetX()[k]) > 1e-9){
            std::cout << "Points do not match in x for getRatio" << std::endl; 
            delete theRatio; 
            return nullptr;
        }
        if (den->GetY()[k] != 0) {
            theRatio->SetPoint(theRatio->GetN(), den->GetX()[k], num->GetY()[k] / den->GetY()[k]);
        }
    }
    return theRatio;
}

TGraph* cropToFrame(TGraph* input, double xmin = std::numeric_limits<double>::min(), double xmax = std::numeric_limits<double>::max()) {
    TGraph *newGraph = new TGraph();  
    for (int k = 0; k < input->GetN(); ++k) {
        double x = input->GetX()[k]; 
        if (x >= xmin && x <= xmax){
            newGraph->SetPoint(newGraph->GetN(), x, input->GetY()[k]); 
        }
    }
    return newGraph; 
}

void drawDiskUsageSplitPad(Plot<TGraph> & graphR21, Plot<TGraph> & graphR22, Plot<TGraph> & graphR22NoLRT) {
    graphR21.populate();
    graphR22.populate();
    graphR22NoLRT.populate();

    auto ratio      = getRatio(graphR22.getHisto(), graphR21.getHisto());
    auto ratioNoLRT = getRatio(graphR22NoLRT.getHisto(), graphR21.getHisto());

    // Get our colors
    Int_t colR21 = TColor::GetFreeColorIndex();
    TColor* c1 = new TColor(colR21, 0.42,0.70,0.32);

    Int_t colR22Full  = TColor::GetFreeColorIndex();
    TColor* c2 = new TColor(colR22Full, 0.12,0.47,0.71);

    Int_t colR22NoLRT = TColor::GetFreeColorIndex();
    TColor* c3 = new TColor(colR22NoLRT, 0.65,0.22,0.83);
    //

    graphR21->SetMarkerStyle(kFullSquare);
    graphR21->SetMarkerColor(colR21);
    graphR21->SetLineColor(colR21);
    graphR21->SetLineWidth(2);

    graphR22->SetMarkerStyle(kFullDotLarge);
    graphR22->SetMarkerColor(colR22Full);
    graphR22->SetLineColor(colR22Full);
    graphR22->SetLineWidth(2);

    graphR22NoLRT->SetMarkerStyle(kOpenCircle);
    graphR22NoLRT->SetMarkerColor(colR22NoLRT);
    graphR22NoLRT->SetLineColor(colR22NoLRT);
    graphR22NoLRT->SetLineWidth(2);

    /// where to split in pad coordinates
    double horizontalSplit = 0.8; 
    /// where to stop the left pad
    double leftMax = 62.;
    /// where to start the right pad 
    double rightMin = 78.; 

    /// ATLAS style 
    SetAtlasStyle();
    double start = graphR21->GetX()[0];
    double end   = graphR21->GetX()[graphR21->GetN()-1]; 
    
    TGraph* graphR21_left       = cropToFrame(graphR21.getHisto(), start, leftMax); 
    TGraph* graphR22_left       = cropToFrame(graphR22.getHisto(), start, leftMax); 
    TGraph* graphR22NoLRT_left  = cropToFrame(graphR22NoLRT.getHisto(), start, leftMax); 

    TGraph* graphR21_right      = cropToFrame(graphR21.getHisto(), rightMin, end); 
    TGraph* graphR22_right      = cropToFrame(graphR22.getHisto(), rightMin, end); 
    TGraph* graphR22NoLRT_right = cropToFrame(graphR22NoLRT.getHisto(), rightMin, end); 

    TGraph* ratio_left      = cropToFrame(ratio, start, leftMax); 
    TGraph* ratioNoLRT_left = cropToFrame(ratioNoLRT, start, leftMax); 

    TGraph* ratio_right      = cropToFrame(ratio, rightMin, end); 
    TGraph* ratioNoLRT_right = cropToFrame(ratioNoLRT, rightMin, end); 

    /// Determine required axis ranges 
    double min=1e20; double max = -1e20; 
    double rmin=1e20; double rmax = -1e20; 
    for (int k =0 ; k < graphR21->GetN(); ++k){
        if (graphR21->GetY()[k] > max) max = graphR21->GetY()[k]; 
        if (graphR21->GetY()[k] < min) min = graphR21->GetY()[k]; 
    }
    for (int k =0 ; k < graphR22->GetN(); ++k){
        if (graphR22->GetY()[k] > max) max = graphR22->GetY()[k]; 
        if (graphR22->GetY()[k] < min) min = graphR22->GetY()[k]; 
    }
    for (int k =0 ; k < graphR22NoLRT->GetN(); ++k){
        if (graphR22NoLRT->GetY()[k] > max) max = graphR22NoLRT->GetY()[k]; 
        if (graphR22NoLRT->GetY()[k] < min) min = graphR22NoLRT->GetY()[k]; 
    }
    for (int k =0 ; k < ratio->GetN(); ++k){
        if (ratio->GetY()[k] > rmax) rmax = ratio->GetY()[k]; 
        if (ratio->GetY()[k] < rmin) rmin = ratio->GetY()[k]; 
    }
    for (int k =0 ; k < ratioNoLRT->GetN(); ++k){
        if (ratioNoLRT->GetY()[k] > rmax) rmax = ratioNoLRT->GetY()[k]; 
        if (ratioNoLRT->GetY()[k] < rmin) rmin = ratioNoLRT->GetY()[k]; 
    }
    
    TCanvas* can = new TCanvas("can","can",800,600); 
    can->cd();

    /// Start booking pads. Arrangement on the canvas: 
    ///   1  | 2   main part
    ///  ----------------
    ///   3  | 4   ratio

    double verticalSplit = 0.4; 

    TPad* subp1 = new TPad("sub1","sub1",0,verticalSplit,horizontalSplit,1);
    subp1->SetLeftMargin(0.12);
    subp1->SetBottomMargin(0); 
    subp1->SetTopMargin(0.12); 
    subp1->SetRightMargin(0.015);
    subp1->SetFillColorAlpha(0,0); 
    subp1->SetLineColorAlpha(0,0); 
    subp1->SetTicks(1,1);
    subp1->Draw(); 

    TPad* subp2  = new TPad("sub2","sub2",horizontalSplit,verticalSplit,1,1);
    subp2->SetLeftMargin(0.015);
    subp2->SetBottomMargin(subp1->GetBottomMargin()); 
    subp2->SetTopMargin(subp1->GetTopMargin()); 
    subp2->SetRightMargin(0.35);
    subp2->SetTicks(1,1);
    subp2->SetDrawOption("Y+");
    subp2->SetLineColor(kGray+3);
    subp2->Draw(); 

    TPad* subp3 = new TPad("sub3","sub3",0,0,horizontalSplit,verticalSplit);
    subp3->SetLeftMargin(subp1->GetLeftMargin());
    subp3->SetRightMargin(subp1->GetRightMargin());
    subp3->SetBottomMargin(0.4); 
    subp3->SetTopMargin(0.0); 
    subp3->SetFillColorAlpha(0,0); 
    subp3->SetLineColorAlpha(0,0); 
    subp3->SetTicks(1,1);
    subp3->SetGridy(); 
    subp3->Draw(); 

    TPad* subp4 = new TPad("sub4","sub4",horizontalSplit,0, 1, verticalSplit);
    subp4->SetLeftMargin(subp2->GetLeftMargin());
    subp4->SetRightMargin(subp2->GetRightMargin());
    subp4->SetBottomMargin(subp3->GetBottomMargin()); 
    subp4->SetTopMargin(subp3->GetTopMargin()); 
    subp4->SetTicks(1,0);
    subp4->SetGridy(); 
    subp4->SetLogx();
    subp4->Draw(); 

    subp1->cd(); 
    /// Y Axis range is the same for left and right
    TH1* topFrameLeft = subp1->DrawFrame(start,-15,leftMax,max*1.05); 
    topFrameLeft->GetYaxis()->SetTitleFont(43); 
    topFrameLeft->GetYaxis()->SetTitleSize(20); 
    topFrameLeft->GetYaxis()->SetLabelFont(43); 
    topFrameLeft->GetYaxis()->SetLabelSize(20); 
    topFrameLeft->GetYaxis()->SetTitle("Disk Space [kb/event]"); 

    graphR21_left->Draw("PLSAME");
    graphR21_left->SetMarkerStyle(kFullSquare);
    graphR21_left->SetMarkerColor(colR21);
    graphR21_left->SetLineColor(colR21);
    graphR21_left->SetLineWidth(2);
    
    graphR22_left->Draw("PLSAME");
    graphR22_left->SetMarkerStyle(kFullDotLarge);
    graphR22_left->SetMarkerColor(colR22Full);
    graphR22_left->SetLineColor(colR22Full);
    graphR22_left->SetLineWidth(2);
    
    graphR22NoLRT_left->Draw("PLSAME");
    graphR22NoLRT_left->SetMarkerStyle(kOpenCircle);
    graphR22NoLRT_left->SetMarkerColor(colR22NoLRT);
    graphR22NoLRT_left->SetLineColor(colR22NoLRT);
    graphR22NoLRT_left->SetLineWidth(2);

    topFrameLeft->Draw("SAMEaxis");

    subp2->cd(); 
    TH1* topFrameRight = subp2->DrawFrame(rightMin,-15,end+2,max*1.05); 
    topFrameRight->GetYaxis()->SetLabelFont(43); 
    topFrameRight->SetDrawOption("Y+");
    topFrameRight->SetDrawOption("Y+");
    topFrameRight->GetXaxis()->SetNdivisions(305);
    topFrameRight->GetYaxis()->SetTitleSize(0); 
    topFrameRight->GetYaxis()->SetLabelSize(topFrameLeft->GetYaxis()->GetLabelSize()); 
    topFrameRight->SetAxisColor(kGray+2);
    topFrameRight->SetLabelColor(kGray+2);
    topFrameRight->GetYaxis()->SetAxisColor(kGray+2);
    topFrameRight->GetYaxis()->SetLabelColor(kGray+2);
    topFrameRight->Draw("Y+"); 
    
    PlotUtils::drawTLatex(0.05, 0.07, "LHC Fill 7358", 16, 43, 11, kGray+3);
    PlotUtils::drawTLatex(0.05, 0.02, "Not on GRL", 16 , 53, 11, kGray+3);
    TBox theBox(topFrameRight->GetXaxis()->GetXmin(), topFrameRight->GetYaxis()->GetXmin(), topFrameRight->GetXaxis()->GetXmax(), topFrameRight->GetYaxis()->GetXmax()); 
    theBox.SetFillStyle(3004); 
    theBox.SetFillColor(kGray+2); 
    theBox.Draw();
    
    graphR21_right->Draw("PLSAME");
    graphR21_right->SetMarkerStyle(kFullSquare);
    graphR21_right->SetMarkerColor(colR21);
    graphR21_right->SetLineColor(colR21);
    graphR21_right->SetLineWidth(2);
    
    graphR22_right->Draw("PLSAME");
    graphR22_right->SetMarkerStyle(kFullDotLarge);
    graphR22_right->SetMarkerColor(colR22Full);
    graphR22_right->SetLineColor(colR22Full);
    graphR22_right->SetLineWidth(2);
    
    graphR22NoLRT_right->Draw("PLSAME");
    graphR22NoLRT_right->SetMarkerStyle(kOpenCircle);
    graphR22NoLRT_right->SetMarkerColor(colR22NoLRT);
    graphR22NoLRT_right->SetLineColor(colR22NoLRT);
    graphR22NoLRT_right->SetLineWidth(2);

    topFrameRight->Draw("SAMEaxisY+");
    gPad->RedrawAxis();

    topFrameLeft->SetLineColorAlpha(0, 0);
    topFrameRight->SetLineColorAlpha(0, 0);
    subp1->SetLineColorAlpha(0, 0);
    subp2->SetLineColorAlpha(0, 0);
    subp1->Update();
    subp2->Update();

    // on to the ratios! 
    subp3->cd(); 
    
    TH1*rFrameLeft = subp3->DrawFrame(start, rmin*0.8,leftMax,rmax*1.2);
    rFrameLeft->GetYaxis()->SetTitleFont(43);
    rFrameLeft->GetYaxis()->SetLabelFont(43);
    rFrameLeft->GetXaxis()->SetTitleFont(43);
    rFrameLeft->GetXaxis()->SetLabelFont(43);
    rFrameLeft->GetYaxis()->SetTitleSize(topFrameLeft->GetYaxis()->GetTitleSize());
    rFrameLeft->GetYaxis()->SetLabelSize(topFrameLeft->GetYaxis()->GetLabelSize());
    rFrameLeft->GetXaxis()->SetTitleSize(topFrameLeft->GetYaxis()->GetTitleSize());
    rFrameLeft->GetXaxis()->SetLabelSize(topFrameLeft->GetYaxis()->GetLabelSize());
    rFrameLeft->GetYaxis()->SetNdivisions(505); 
    rFrameLeft->GetXaxis()->SetTitleOffset(1.5 * rFrameLeft->GetXaxis()->GetTitleOffset());
    rFrameLeft->GetYaxis()->SetTitle("Run 3 / Run 2");

    ratio_left->Draw("PLSAME"); 
    ratio_left->SetMarkerStyle(kFullDotLarge);
    ratio_left->SetMarkerColor(colR22Full);
    ratio_left->SetLineColor(colR22Full);
    ratio_left->SetLineWidth(2);
    
    ratioNoLRT_left->Draw("PLSAME"); 
    ratioNoLRT_left->SetMarkerStyle(kOpenCircle);
    ratioNoLRT_left->SetMarkerColor(colR22NoLRT);
    ratioNoLRT_left->SetLineColor(colR22NoLRT);
    ratioNoLRT_left->SetLineWidth(2);

    gPad->RedrawAxis();
    
    subp4->cd(); 
    TH1* rFrameRight = subp4->DrawFrame(rightMin, rmin*0.8, end+2, rmax*1.2);
    rFrameRight->GetYaxis()->SetNdivisions(505); 
    rFrameRight->GetXaxis()->SetNdivisions(305); 
    rFrameRight->GetYaxis()->SetTitleFont(43);
    rFrameRight->GetYaxis()->SetLabelFont(43);
    rFrameRight->GetXaxis()->SetTitleFont(43);
    rFrameRight->GetXaxis()->SetLabelFont(43);
    rFrameRight->GetYaxis()->SetTitleSize(topFrameLeft->GetYaxis()->GetTitleSize());
    rFrameRight->GetYaxis()->SetLabelSize(topFrameLeft->GetYaxis()->GetLabelSize());
    rFrameRight->GetXaxis()->SetTitleSize(topFrameLeft->GetYaxis()->GetTitleSize());
    rFrameRight->GetXaxis()->SetLabelSize(topFrameLeft->GetYaxis()->GetLabelSize());
    rFrameRight->GetXaxis()->SetTitleOffset(1.5 * rFrameRight->GetXaxis()->GetTitleOffset());
    rFrameRight->GetYaxis()->SetTitleOffset(1.5 * rFrameRight->GetYaxis()->GetTitleOffset()); 
    rFrameRight->GetXaxis()->SetTitle("#LT#mu#GT"); 
    rFrameRight->SetDrawOption("Y+");
    rFrameRight->SetLineWidth(0);
    rFrameRight->GetYaxis()->SetTitleSize(0);
    rFrameRight->GetXaxis()->SetLabelSize(0); 
    rFrameRight->SetAxisColor(kGray+2);
    rFrameRight->SetLabelColor(kGray+2);
    rFrameRight->GetYaxis()->SetAxisColor(kGray+2);
    rFrameRight->GetYaxis()->SetLabelColor(kGray+2);
    rFrameRight->GetXaxis()->SetAxisColor(kGray+2);
    rFrameRight->GetXaxis()->SetLabelColor(kGray+2);
    rFrameRight->GetXaxis()->SetTitleColor(kBlack);
    rFrameRight->GetXaxis()->SetTitleOffset(2.35);
    rFrameRight->Draw("Y+");
    subp4->Update();

    TBox ratioBox(rightMin, rFrameRight->GetYaxis()->GetXmin(), rFrameRight->GetXaxis()->GetXmax(), rFrameRight->GetYaxis()->GetXmax()); 
    ratioBox.SetFillStyle(3004); 
    ratioBox.SetFillColor(kGray+2); 
    ratioBox.Draw();

    ratio_right->Draw("PLSAME");
    ratio_right->SetMarkerStyle(kFullDotLarge);
    ratio_right->SetMarkerColor(colR22Full);
    ratio_right->SetLineColor(colR22Full);
    ratio_right->SetLineWidth(2);

    ratioNoLRT_right->Draw("PLSAME");
    ratioNoLRT_right->SetMarkerStyle(kOpenCircle);
    ratioNoLRT_right->SetMarkerColor(colR22NoLRT);
    ratioNoLRT_right->SetLineColor(colR22NoLRT);
    ratioNoLRT_right->SetLineWidth(2);
    
    TLatex* tl = new TLatex(); 
    tl->SetTextFont(43);
    tl->SetTextSize(rFrameLeft->GetXaxis()->GetLabelSize());
    tl->SetTextColor(kGray+2);
    tl->SetTextAlign(23);
    tl->SetNDC(false); 
    tl->DrawLatex(80, rFrameRight->GetMinimum() - 0.02 * (rFrameRight->GetMaximum() - rFrameRight->GetMinimum()),"80"); 
    tl->DrawLatex(90, rFrameRight->GetMinimum() - 0.02 * (rFrameRight->GetMaximum() - rFrameRight->GetMinimum()),"90"); 
    subp4->Update();

    CanvasOptions canvasOpts = CanvasOptions();

    subp1->cd(); 
    canvasOpts = canvasOpts.doAtlasLabel(true).doLumiLabel(false).fontSize(24).atlasLabelX(0.04).atlasLabelY(-0.09).labelStatusTag("Preliminary");
    canvasOpts.drawLabels({});

    canvasOpts = canvasOpts.doAtlasLabel(false).doLumiLabel(false).fontSize(22).otherLabelX(0.04).otherLabelStartY(-0.16);
    canvasOpts.drawLabels({"Inner Detector only", "LHC Fill 6291"});

    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{
        PlotUtils::LegendEntry(graphR21(),      "Run 2 Reconstruction",         "PL"),
        PlotUtils::LegendEntry(graphR22(),      "Run 3 Reconstruction",         "PL"),
        PlotUtils::LegendEntry(graphR22NoLRT(), "Run 3 Reconstruction w/o LRT", "PL"),
    },
        0.20, 0.40, 0.55, 0.60
    );
    
    canvasOpts.SaveCanvas(can, "Size_vs_mu_run2_vs_run3_splitx");

    delete c1;
    delete c2;
    delete c3;
}

void drawDiskUsage(Plot<TGraph> & graphR21, Plot<TGraph> & graphR22, Plot<TGraph> & graphR22NoLRT) {
    graphR21.populate();
    graphR22.populate();
    graphR22NoLRT.populate();

    Plot<TGraph> graphRatio("", getRatio(graphR22(), graphR21()));
    Plot<TGraph> graphRatioNoLRT("", getRatio(graphR22NoLRT(), graphR21()));

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
    canvasOpts = canvasOpts.doAtlasLabel(true).doLumiLabel(false).fontSize(24).atlasLabelX(0.04).atlasLabelY(-0.09).labelStatusTag("Preliminary");
    canvasOpts.drawLabels({});

    canvasOpts = canvasOpts.doAtlasLabel(false).doLumiLabel(false).fontSize(22).otherLabelX(0.04).otherLabelStartY(-0.16);
    canvasOpts.drawLabels({"Run 2 Data Reconstruction", "ATLAS ID Only"});

    PlotUtils::drawLegend(std::vector<PlotUtils::LegendEntry>{
        PlotUtils::LegendEntry(graphR21(),      "Run 2 Reconstruction",         "PL"),
        PlotUtils::LegendEntry(graphR22(),      "Run 3 Reconstruction",         "PL"),
        PlotUtils::LegendEntry(graphR22NoLRT(), "Run 3 Reconstruction w/o LRT", "PL"),
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
    
    graphRatio->GetXaxis()->SetTitle("#LT#mu#GT");
    graphRatio->GetYaxis()->SetTitle("Run 3 / Run 2");
    graphRatio->GetXaxis()->SetTitleFont(43);
    graphRatio->GetYaxis()->SetTitleFont(43);
    graphRatio->GetXaxis()->SetLabelFont(43);
    graphRatio->GetYaxis()->SetLabelFont(43);
    graphRatio->GetXaxis()->SetTitleSize(24); 
    graphRatio->GetYaxis()->SetTitleSize(22); 
    graphRatio->GetXaxis()->SetLabelSize(22); 
    graphRatio->GetYaxis()->SetLabelSize(22); 

    graphRatio->GetXaxis()->SetTitleOffset(2.25);
    graphRatio->GetYaxis()->SetTitleOffset(1.05);

    canvasOpts.SaveCanvas(mpc.getCanvas(), "Size_vs_mu_run2_vs_run3");

    delete c1;
    delete c2;
    delete c3;
}

int main (int, char**) {

    SetAtlasStyle();

    const std::string tree  = "Squirrels";
    const std::string inR21 = "/home/zschilla/Working/IDPVMAnalysis/run/SPOT/SPOTResults.Serhan.R21.root";        
    const std::string inR22 = "/home/zschilla/Working/IDPVMAnalysis/run/SPOT/SPOTResults.Serhan.R22.root";

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

    Plot<TGraph> graphR21("graphR21", std::make_shared<TGraph>(r21muDisk.first.size(), &(r21muDisk.first[0]), &(r21muDisk.second[0])).get());
    Plot<TGraph> graphR22("graphR22", std::make_shared<TGraph>(r22muDisk.first.size(), &(r22muDisk.first[0]), &(r22muDisk.second[0])).get());
    Plot<TGraph> graphR22NoLRT("graphR22NoLRT", std::make_shared<TGraph>(r22muDiskNoLRT.first.size(), &(r22muDiskNoLRT.first[0]), &(r22muDiskNoLRT.second[0])).get());

    drawDiskUsage(graphR21, graphR22, graphR22NoLRT);
    drawDiskUsageSplitPad(graphR21, graphR22, graphR22NoLRT);

    return EXIT_SUCCESS;
}
