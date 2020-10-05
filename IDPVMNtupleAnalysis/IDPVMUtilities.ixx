#include "TFile.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

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

    Plot<HistoType> thePlot(GetPlotTitle(plotname), h_out);
    delete h_out;

    return thePlot;
}

template <class HistoType> std::map<std::string, Plot<HistoType>> LoadIDPVMHistograms(const std::string & myphysval, const std::vector<std::string> & plotnames) {

    TFile* theFile = TFile::Open(myphysval.c_str(), "READ");

    std::map<std::string, Plot<HistoType>> theHistograms{};
    for (auto & pname : plotnames) {
        std::cout << "Reading histogram " << pname << " from file " << myphysval << std::endl;
        
        HistoType* h_out = GetHistogram<HistoType>(theFile, pname);

        theHistograms.emplace(pname, Plot<HistoType>(GetPlotTitle(pname), h_out));
        delete h_out;
    }

    delete theFile;

    return theHistograms;
}
template <class HistoIn, class HistoOut> Plot<HistoOut> LoadIDPVMHistogram(const std::string & myphysval, const std::string & plotname) {
    
    Plot<HistoIn> h_temp = LoadIDPVMHistogram<HistoIn>(myphysval, plotname);
    return Plot<HistoOut>(GetPlotTitle(plotname), dynamic_cast<HistoOut*>(h_temp()));
}