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
        d0=0,
        z0=1,
        phi=2,
        theta=3,
        qOverPt=4,
        pt=5,
        eta=6,
    } variable;
}

namespace IDPVMLabels {
    const std::string getResolutionLabel(IDPVMDefs::variable);
    const std::string getPullWidthLabel(IDPVMDefs::variable);
    const std::string getPullMeanLabel(IDPVMDefs::variable);
}

namespace IDPVMTemplates {
    const std::vector<double> populateLogLinearBinning(int nBins, double absXmin, double absXmax, bool symmetriseAroundZero = true);
    const std::vector<double> getResolutionBinning(IDPVMDefs::variable var); 

    TH2D getResolutionHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus); 
    TH2D getPullHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus);

    TH1D getEfficiencyHistTemplate(IDPVMDefs::variable var); 
}

#endif


