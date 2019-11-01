#ifndef __IDPVMTEMPLATES__H
#define __IDPVMTEMPLATES__H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "TH2D.h"

namespace IDPVMDefs {
    typedef enum {
        eta=0,
        d0=1,
        z0=2,
        phi=3,
        theta=4,
    } variable;
    typedef enum {
        null=0,
        resolution=1,
        efficiency=2,
    } plot;
}

namespace IDPVMLabels {
    const std::string getResolutionLabel(IDPVMDefs::variable);
}

namespace IDPVMTemplates {
    const std::vector<double> get1DBinning(IDPVMDefs::variable var, IDPVMDefs::plot plt = IDPVMDefs::null); 
    TH2D getResolutionHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus = IDPVMDefs::eta); 
}

#endif


