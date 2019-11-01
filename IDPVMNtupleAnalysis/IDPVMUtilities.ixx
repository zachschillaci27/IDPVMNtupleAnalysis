#include "TFile.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

template <class HistoType> void CheckHistogram(HistoType* histo) {
    if (histo->GetEntries() != 0) {
        std::cout << "Success! Histogram has non-zero entries." << std::endl;
    }
    else {
        std::cout << "Failure! Histogram is empty." << std::endl;
    }
}

template <class HistoType> HistoType* ReadHistogram(const std::string & myphysval, const std::string & plotname) {

    std::cout << "Reading histogram " << plotname << " from file " << myphysval << std::endl;

    TFile* theFile = TFile::Open(myphysval.c_str(), "READ");

    HistoType* h_temp = dynamic_cast<HistoType*>(theFile->Get(plotname.c_str()));    
    HistoType* h_out = dynamic_cast<HistoType*>(h_temp->Clone());
    h_out->SetDirectory(0);
    CheckHistogram(h_out);

    delete theFile;

    return h_out;
}

template <class HistoType> std::map<std::string, HistoType*> ReadHistograms(const std::string & myphysval, const std::vector<std::string> & plotnames) {

    TFile* theFile = TFile::Open(myphysval.c_str(), "READ");

    std::map<std::string, HistoType*> theHistograms = {};
    for (auto & pname : plotnames) {
        std::cout << "Reading histogram " << pname << " from file " << myphysval << std::endl;

        HistoType* h_temp = dynamic_cast<HistoType*>(theFile->Get(pname.c_str()));    
        HistoType* h_out = dynamic_cast<HistoType*>(h_temp->Clone());
        h_out->SetDirectory(0);
        CheckHistogram(h_out);

        theHistograms[pname] = h_out;
    }

    delete theFile;

    return theHistograms;
}