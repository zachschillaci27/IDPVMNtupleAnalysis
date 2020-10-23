#ifndef __IDPVMPLOTUTILS__H
#define __IDPVMPLOTUTILS__H

#include "NtupleAnalysisUtils/PlotUtils.h"
#include "NtupleAnalysisUtils/CanvasOptions.h"
#include "NtupleAnalysisUtils/DefaultPlotting.h"

namespace IDPVMPlotUtils {
    void CompareWithIDPVM(Plot<TH1> nominal, Plot<TH1> alternative, const std::vector<std::string> & labels);
    template <typename AltHist> void CompareWithIDPVM(Plot<TH1> nominal, Plot<AltHist> alternative, const std::vector<std::string> & labels);
}

template <typename AltHist> void IDPVMPlotUtils::CompareWithIDPVM(Plot<TH1> nominal, Plot<AltHist> alternative, const std::vector<std::string> & labels) {
    CompareWithIDPVM(nominal, Plot<TH1>("", dynamic_cast<TH1*>(alternative())), labels);
}

#endif


