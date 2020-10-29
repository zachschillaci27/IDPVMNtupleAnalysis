#include "IDPVMNtupleAnalysis/IDPVMUtilities.h"

const std::string GetPlotTitle(const std::string & plotname) {
    return plotname.substr(plotname.find_last_of("/") + 1);
}

Plot<TH1> LoadIDPVMEfficiency(const std::string & myphysval, const std::string & plotname) {

    // Load as TEfficiency
    Plot<TEfficiency> h_temp = LoadIDPVMHistogram<TEfficiency>(myphysval, plotname);

    // Convert to TH1
    std::shared_ptr<TH1> h_out(PlotUtils::getRatio(h_temp->GetCopyPassedHisto(), h_temp->GetCopyTotalHisto(), PlotUtils::efficiencyErrors));
    Plot<TH1> thePlot(GetPlotTitle(plotname), h_out.get());

    return thePlot;
}

Plot<TEfficiency> RebinEfficiency(Plot<TEfficiency> & efficiencyIn, int rebin) {

    Plot<TH1D> passed("passed", dynamic_cast<TH1D*>(efficiencyIn->GetCopyPassedHisto()));
    Plot<TH1D> total("total", dynamic_cast<TH1D*>(efficiencyIn->GetCopyTotalHisto()));

    passed->Rebin(rebin);
    total->Rebin(rebin);

    std::shared_ptr<TEfficiency> teffOut = std::make_shared<TEfficiency>(*passed(), *total());
    Plot<TEfficiency> efficiencyOut("efficiencyOut", teffOut.get());
    
    efficiencyOut.setPlotFormat(efficiencyIn.plotFormat());
    efficiencyOut.setLegendTitle(efficiencyIn.getLegendTitle());
    efficiencyOut.setLegendOption(efficiencyIn.getLegendOption());
    efficiencyOut.applyFormat();

    return efficiencyOut;
}

Plot<TEfficiency> SymmetrizeEfficiency(Plot<TEfficiency> & efficiencyIn) {

    Plot<TH1D> passed("passed", dynamic_cast<TH1D*>(efficiencyIn->GetCopyPassedHisto()));
    Plot<TH1D> total("total", dynamic_cast<TH1D*>(efficiencyIn->GetCopyTotalHisto()));

    Plot<TH1D> passed_sym("passed_sym", dynamic_cast<TH1D*>(efficiencyIn->GetCopyPassedHisto()));
    Plot<TH1D> total_sym("total_sym", dynamic_cast<TH1D*>(efficiencyIn->GetCopyTotalHisto()));
    
    int nbins = passed->GetXaxis()->GetNbins();
    int nbins_sym = nbins / 2;
    
    // std::cout << "Symmetrizing efficiency ..." << std::endl; 
    for (int i = 1; i <= nbins_sym; ++i) {
        int neg_bin = i;
        int pos_bin = nbins + 1 - i;

        // std::cout << "Merging bins " << neg_bin << " and " << pos_bin << std::endl;
        // std::cout << Form("(%.2f, %.2f)", passed->GetXaxis()->GetBinLowEdge(neg_bin), passed->GetXaxis()->GetBinUpEdge(neg_bin)) << " + "
        //           << Form("(%.2f, %.2f)", passed->GetXaxis()->GetBinLowEdge(pos_bin), passed->GetXaxis()->GetBinUpEdge(pos_bin)) << std::endl;

        passed_sym->SetBinContent(pos_bin, passed->GetBinContent(neg_bin) + passed->GetBinContent(pos_bin));
        passed_sym->SetBinContent(neg_bin, 0.);

        total_sym->SetBinContent(pos_bin, total->GetBinContent(neg_bin) + total->GetBinContent(pos_bin));
        total_sym->SetBinContent(neg_bin, 0.);
    }

    std::shared_ptr<TEfficiency> teffOut = std::make_shared<TEfficiency>(*passed_sym(), *total_sym());
    Plot<TEfficiency> efficiencyOut("efficiencyOut", teffOut.get());
    
    efficiencyOut.setPlotFormat(efficiencyIn.plotFormat());
    efficiencyOut.setLegendTitle(efficiencyIn.getLegendTitle());
    efficiencyOut.setLegendOption(efficiencyIn.getLegendOption());
    efficiencyOut.applyFormat();

    return efficiencyOut;
}

Plot<TProfile> SymmetrizeProfile(Plot<TProfile> & profileIn) {
    
    Plot<TProfile> profileOut("profileOut", profileIn);
    profileOut.setPlotFormat(profileIn.plotFormat());
    profileOut.setLegendTitle(profileIn.getLegendTitle());
    profileOut.setLegendOption(profileIn.getLegendOption());
    profileOut.applyFormat();

    int nbins = profileIn->GetXaxis()->GetNbins();
    int nbins_sym = nbins / 2;
    
    std::cout << "Symmetrizing profile ..." << std::endl; 
    for (int i = 1; i <= nbins_sym; ++i) {
        int neg_bin = i;
        int pos_bin = nbins + 1 - i;

        // std::cout << "Merging bins " << neg_bin << " and " << pos_bin << std::endl;
        // std::cout << Form("(%.2f, %.2f)", profileIn->GetXaxis()->GetBinLowEdge(neg_bin), profileIn->GetXaxis()->GetBinUpEdge(neg_bin)) << " + "
        //           << Form("(%.2f, %.2f)", profileIn->GetXaxis()->GetBinLowEdge(pos_bin), profileIn->GetXaxis()->GetBinUpEdge(pos_bin)) << std::endl;

        profileOut->SetBinEntries(pos_bin, 0.5 * (profileIn->GetBinEntries(neg_bin) + profileIn->GetBinEntries(pos_bin)));
        profileOut->SetBinEntries(neg_bin, 0.);
    }

    return profileOut;
}

Plot<TH1F> SymmetrizeResolution(Plot<TH1F> & resolutionIn) {
    
    Plot<TH1F> resolutionOut("resolutionOut", resolutionIn);
    resolutionOut.setPlotFormat(resolutionIn.plotFormat());
    resolutionOut.setLegendTitle(resolutionIn.getLegendTitle());
    resolutionOut.setLegendOption(resolutionIn.getLegendOption());
    resolutionOut.applyFormat();

    int nbins = resolutionIn->GetXaxis()->GetNbins();
    int nbins_sym = nbins / 2;
    
    std::cout << "Symmetrizing resolution ..." << std::endl; 
    for (int i = 1; i <= nbins_sym; ++i) {
        int neg_bin = i;
        int pos_bin = nbins + 1 - i;

        // std::cout << "Merging bins " << neg_bin << " and " << pos_bin << std::endl;
        // std::cout << Form("(%.2f, %.2f)", resolutionIn->GetXaxis()->GetBinLowEdge(neg_bin), resolutionIn->GetXaxis()->GetBinUpEdge(neg_bin)) << " + "
        //           << Form("(%.2f, %.2f)", resolutionIn->GetXaxis()->GetBinLowEdge(pos_bin), resolutionIn->GetXaxis()->GetBinUpEdge(pos_bin)) << std::endl;

        resolutionOut->SetBinContent(pos_bin, 0.5 * (resolutionIn->GetBinContent(neg_bin) + resolutionIn->GetBinContent(pos_bin)));
    }

    return resolutionOut;
}