#ifndef __IDPVMTEMPLATES__H
#define __IDPVMTEMPLATES__H

#include "TH1D.h"
#include "TH2D.h"
#include <algorithm>    // std::transform, std::reverse
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace IDPVMDefs {
    typedef enum {
        eta=0,
        pt=1,
        d0=2,
        z0=3,
        z0sin=4, 
        phi=5,
        theta=6,
        qOverPt=7,
        qOverP=8,
    } variable;
}

namespace IDPVMLabels {
    const std::string getVarName(IDPVMDefs::variable);
    const std::string getResolutionLabel(IDPVMDefs::variable);
    const std::string getPullWidthLabel(IDPVMDefs::variable);
    const std::string getPullMeanLabel(IDPVMDefs::variable);
}

namespace IDPVMTemplates {
    const std::vector<double> populateLinearBinning(int nBins, double xMin, double xMax);
    const std::vector<double> populateLogLinearBinning(int nBins, double absXmin, double absXmax, bool symmetriseAroundZero = true);
    const std::vector<double> getResolutionBinning(IDPVMDefs::variable var); 

    TH2D getResolutionHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus); 
    TH2D getPullHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus);

    TH1D getEfficiencyHistTemplate(IDPVMDefs::variable var); 
    TH1D getFakeRateHistTemplate(IDPVMDefs::variable var); 
}

#endif


