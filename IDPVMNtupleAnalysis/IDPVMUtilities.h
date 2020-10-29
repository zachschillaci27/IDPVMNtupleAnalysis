#ifndef __IDPVMUTILITIES__H
#define __IDPVMUTILITIES__H

#include "NtupleAnalysisUtils/Plot.h"
#include "NtupleAnalysisUtils/PlotUtils.h"
#include "TFile.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

// Histogram loading
const std::string GetPlotTitle(const std::string & plotname);

template <class HistoType> std::shared_ptr<HistoType> GetHistogram(std::shared_ptr<TFile> theFile, const std::string & plotname);
template <class HistoType> Plot<HistoType> LoadIDPVMHistogram(const std::string & myphysval, const std::string & plotname);
template <class HistoType> std::map<std::string, Plot<HistoType>> LoadIDPVMHistograms(const std::string & myphysval, const std::vector<std::string> & plotnames);
template <class HistoIn, class HistoOut> Plot<HistoOut> LoadIDPVMHistogram(const std::string & myphysval, const std::string & plotname);

// Histogram manipulation/rebinning/symmetrization
Plot<TH1> LoadIDPVMEfficiency(const std::string & myphysval, const std::string & plotname);
Plot<TEfficiency> RebinEfficiency(Plot<TEfficiency> & efficiencyIn, int rebin);
Plot<TEfficiency> SymmetrizeEfficiency(Plot<TEfficiency> & efficiencyIn);
Plot<TProfile> SymmetrizeProfile(Plot<TProfile> & profileIn);
Plot<TH1F> SymmetrizeResolution(Plot<TH1F> & resolutionIn);

#include "IDPVMUtilities.ixx"

#endif