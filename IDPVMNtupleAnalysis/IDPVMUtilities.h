#ifndef __IDPVMUTILITIES__H
#define __IDPVMUTILITIES__H
#include <iostream>
#include <string>
#include <vector>
#include <map>

template <class HistoType> void CheckHistogram(HistoType* histo);

template <class HistoType> HistoType* ReadHistogram(const std::string & myphysval, const std::string & plotname);

template <class HistoType> std::map<std::string, HistoType*> ReadHistograms(const std::string & myphysval, const std::vector<std::string> & plotnames);

#include "IDPVMUtilities.ixx"

#endif