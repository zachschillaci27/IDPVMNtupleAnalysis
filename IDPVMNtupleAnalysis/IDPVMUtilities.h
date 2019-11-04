#ifndef __IDPVMUTILITIES__H
#define __IDPVMUTILITIES__H

#include "TFile.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

template <class HistoType> HistoType* GetHistogram(TFile* theFile, const std::string & plotname);

template <class HistoType> HistoType* ReadHistogram(const std::string & myphysval, const std::string & plotname);

template <class InHistoType, class OutHistoType> OutHistoType* ReadAndCastHistogram(const std::string & myphysval, const std::string & plotname);

template <class HistoType> std::map<std::string, HistoType*> ReadHistograms(const std::string & myphysval, const std::vector<std::string> & plotnames);

#include "IDPVMUtilities.ixx"

#endif