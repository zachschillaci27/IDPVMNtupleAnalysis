#include "TFile.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

template <class HistoType> std::shared_ptr<HistoType> GetHistogram(std::shared_ptr<TFile> theFile, const std::string & plotname) {

    std::shared_ptr<HistoType> h_temp(dynamic_cast<HistoType*>(theFile->Get(plotname.c_str())));    
    std::shared_ptr<HistoType> h_out(dynamic_cast<HistoType*>(h_temp->Clone()));
    h_out->SetDirectory(0);

    if (h_out) std::cout << "Success!" << std::endl;
    else std::cout << "Failure!" << std::endl;

    return h_out;
}

template <class HistoType> Plot<HistoType> LoadIDPVMHistogram(const std::string & myphysval, const std::string & plotname) {

    std::cout << "Reading histogram " << plotname << " from file " << myphysval << std::endl;

    std::shared_ptr<TFile> theFile(TFile::Open(myphysval.c_str(), "READ"));

    std::shared_ptr<HistoType> h_out = GetHistogram<HistoType>(theFile, plotname);
    Plot<HistoType> thePlot(GetPlotTitle(plotname), h_out.get());

    return thePlot;
}

template <class HistoType> std::map<std::string, Plot<HistoType>> LoadIDPVMHistograms(const std::string & myphysval, const std::vector<std::string> & plotnames) {

    std::shared_ptr<TFile> theFile(TFile::Open(myphysval.c_str(), "READ"));

    std::map<std::string, Plot<HistoType>> theHistograms{};
    for (auto & pname : plotnames) {
        std::cout << "Reading histogram " << pname << " from file " << myphysval << std::endl;
        
        std::shared_ptr<HistoType> h_out = GetHistogram<HistoType>(theFile, pname);
        theHistograms.emplace(pname, Plot<HistoType>(GetPlotTitle(pname), h_out.get()));
    }

    return theHistograms;
}
template <class HistoIn, class HistoOut> Plot<HistoOut> LoadIDPVMHistogram(const std::string & myphysval, const std::string & plotname) {
    
    Plot<HistoIn> h_temp = LoadIDPVMHistogram<HistoIn>(myphysval, plotname);
    return Plot<HistoOut>(GetPlotTitle(plotname), dynamic_cast<HistoOut*>(h_temp()));
}