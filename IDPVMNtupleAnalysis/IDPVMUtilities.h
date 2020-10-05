#ifndef __IDPVMUTILITIES__H
#define __IDPVMUTILITIES__H

#include "NtupleAnalysisUtils/Plot.h"
#include "NtupleAnalysisUtils/PlotUtils.h"
#include "TFile.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

const std::string GetPlotTitle(const std::string & plotname);

template <class HistoType> HistoType* GetHistogram(TFile* theFile, const std::string & plotname);

template <class HistoType> Plot<HistoType> LoadIDPVMHistogram(const std::string & myphysval, const std::string & plotname);

template <class HistoType> std::map<std::string, Plot<HistoType>> LoadIDPVMHistograms(const std::string & myphysval, const std::vector<std::string> & plotnames);

template <class HistoIn, class HistoOut> Plot<HistoOut>  LoadIDPVMHistogram(const std::string & myphysval, const std::string & plotname);

Plot<TH1> LoadIDPVMEfficiency(const std::string & myphysval, const std::string & plotname);

#include "IDPVMUtilities.ixx"

#endif