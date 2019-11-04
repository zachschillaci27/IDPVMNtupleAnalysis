#include "TFile.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

template <class HistoType> HistoType* GetHistogram(TFile* theFile, const std::string & plotname) {

    HistoType* h_temp = dynamic_cast<HistoType*>(theFile->Get(plotname.c_str()));    
    HistoType* h_out = dynamic_cast<HistoType*>(h_temp->Clone());
    h_out->SetDirectory(0);
    
    if (h_out) std::cout << "Success!" << std::endl;
    else std::cout << "Failure!" << std::endl;

    return h_out;
}

template <class HistoType> HistoType* ReadHistogram(const std::string & myphysval, const std::string & plotname) {

    std::cout << "Reading histogram " << plotname << " from file " << myphysval << std::endl;

    TFile* theFile = TFile::Open(myphysval.c_str(), "READ");
    HistoType* h_out = GetHistogram<HistoType>(theFile, plotname);
    delete theFile;

    return h_out;
}

template <class InHistoType, class OutHistoType> OutHistoType* ReadAndCastHistogram(const std::string & myphysval, const std::string & plotname) {

    InHistoType* h_temp = ReadHistogram<InHistoType>(myphysval, plotname);
    OutHistoType* h_out = (OutHistoType*) h_temp->Clone();

    return h_out;
}

template <class HistoType> std::map<std::string, HistoType*> ReadHistograms(const std::string & myphysval, const std::vector<std::string> & plotnames) {

    TFile* theFile = TFile::Open(myphysval.c_str(), "READ");

    std::map<std::string, HistoType*> theHistograms = {};
    for (auto & pname : plotnames) {
        std::cout << "Reading histogram " << pname << " from file " << myphysval << std::endl;
        HistoType* h_out = GetHistogram<HistoType>(theFile, pname);
        theHistograms[pname] = h_out;
    }

    delete theFile;

    return theHistograms;
}