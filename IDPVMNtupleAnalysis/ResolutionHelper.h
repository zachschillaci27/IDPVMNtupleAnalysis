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
 
struct ResolOutput {
    double mean;
    double dMean;
    double RMS;
    double dRMS;
};

ResolOutput GetIterativeRMS(const Plot<TH2D> & hist2D, const unsigned int nbin, bool verbose = false);
Plot<TH1> GetResolution(const Plot<TH2D> & hist2D, IDPVMDefs::variable var);
std::pair<Plot<TH1>, Plot<TH1>> GetPulls(const Plot<TH2D> & hist2D, IDPVMDefs::variable var);

#endif