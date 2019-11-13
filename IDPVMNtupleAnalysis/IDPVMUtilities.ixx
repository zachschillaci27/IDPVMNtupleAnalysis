#include "TFile.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

const std::string GetPlotTitle(const std::string & plotname) {
    std::string plottitle = plotname; 
    plottitle = plottitle.substr(plottitle.find_last_of("/") + 1);
    return plottitle;
}

template <class HistoType> HistoType* GetHistogram(TFile* theFile, const std::string & plotname) {

    HistoType* h_temp = dynamic_cast<HistoType*>(theFile->Get(plotname.c_str()));    
    HistoType* h_out = dynamic_cast<HistoType*>(h_temp->Clone());
    h_out->SetDirectory(0);
    delete h_temp;

    if (h_out) std::cout << "Success!" << std::endl;
    else std::cout << "Failure!" << std::endl;

    return h_out;
}

template <class HistoType> Plot<HistoType> LoadIDPVMHistogram(const std::string & myphysval, const std::string & plotname) {

    std::cout << "Reading histogram " << plotname << " from file " << myphysval << std::endl;

    TFile* theFile = TFile::Open(myphysval.c_str(), "READ");
    HistoType* h_out = GetHistogram<HistoType>(theFile, plotname);
    delete theFile;

    return Plot<HistoType>(GetPlotTitle(plotname), h_out);
}

template <class HistoType> std::map<std::string, Plot<HistoType>> LoadIDPVMHistograms(const std::string & myphysval, const std::vector<std::string> & plotnames) {

    TFile* theFile = TFile::Open(myphysval.c_str(), "READ");

    std::map<std::string, Plot<HistoType>> theHistograms = {};
    for (auto & pname : plotnames) {
        std::cout << "Reading histogram " << pname << " from file " << myphysval << std::endl;
        theHistograms[pname] = Plot<HistoType>(GetPlotTitle(pname), GetHistogram<HistoType>(theFile, pname));
    }

    delete theFile;

    return theHistograms;
}

Plot<TH1> LoadIDPVMEfficiency(const std::string & myphysval, const std::string & effname) {

    Plot<TEfficiency> theTEff = LoadIDPVMHistogram<TEfficiency>(myphysval, effname);
    TH1* theTH1 = PlotUtils::getRatio(theTEff->GetPassedHistogram()->Clone("passed"), theTEff->GetTotalHistogram()->Clone("total"), PlotUtils::efficiencyErrors);

    return Plot<TH1>(GetPlotTitle(effname), theTH1);
}