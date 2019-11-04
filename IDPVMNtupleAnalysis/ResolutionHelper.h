#ifndef __RESOLUTIONHELPER__H
#define __RESOLUTIONHELPER__H
#include "NtupleAnalysisUtils/PlotContent.h"
#include "NtupleAnalysisUtils/PlotUtils.h"
#include "NtupleAnalysisUtils/DefaultPlotting.h"
#include "IDPVMNtupleAnalysis/IDPVMTree.h"
#include "IDPVMNtupleAnalysis/IDPVMTemplates.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include <utility>
 
struct ResOutput {
    double mean;
    double dMean;
    double RMS;
    double dRMS;
};

ResOutput GetIterativeRMS(const Plot<TH2D> & hist2D, const unsigned int nbin, bool verbose = false);
Plot<TH1D> GetResolution(const Plot<TH2D> & hist2D, IDPVMDefs::variable var);
std::pair<Plot<TH1D>, Plot<TH1D>> GetPulls(const Plot<TH2D> & hist2D, IDPVMDefs::variable var);

#endif