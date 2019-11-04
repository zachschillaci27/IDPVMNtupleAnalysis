#ifndef __IDPVMTEMPLATES__H
#define __IDPVMTEMPLATES__H

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
        eta=5,
    } variable;
    typedef enum {
        null=0,
        resolution=1,
        efficiency=2,
    } plot;
}

namespace IDPVMLabels {
    const std::string getResolutionLabel(IDPVMDefs::variable);
    const std::string getPullWidthLabel(IDPVMDefs::variable);
    const std::string getPullMeanLabel(IDPVMDefs::variable);
}

namespace IDPVMTemplates {
    const std::vector<double> populateLogLinearBinning(int nBins, double absXmin, double absXmax, bool symmetriseAroundZero = true);
    const std::vector<double> get1DBinning(IDPVMDefs::variable var, IDPVMDefs::plot plt = IDPVMDefs::null); 
    TH2D getResolutionHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus = IDPVMDefs::eta); 
    TH2D getPullHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus = IDPVMDefs::eta);
}

#endif


