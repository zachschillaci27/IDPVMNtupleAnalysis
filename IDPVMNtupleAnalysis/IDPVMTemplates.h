#ifndef __IDPVMTEMPLATES__H
#define __IDPVMTEMPLATES__H

#include "TH1D.h"
#include "TH2D.h"
#include <algorithm>
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

    typedef enum {
        track=0,
        truth=1
    } level;


    std::vector<IDPVMDefs::variable> getKnownVars();
}

namespace IDPVMLabels {
    const std::string getVarName(IDPVMDefs::variable);
    const std::string getVarName(IDPVMDefs::level);
    const std::string getResolutionLabel(IDPVMDefs::variable);
    const std::string getPullWidthLabel(IDPVMDefs::variable);
    const std::string getPullMeanLabel(IDPVMDefs::variable);
}

namespace Run2 {
    constexpr double PI        = 3.1415926;
    constexpr double ETA       = 2.5;
    constexpr int    NETA      = 64;
    constexpr double D0RES     = 2.0;
    constexpr double Z0RES     = 10.0;
    constexpr double PHIRES    = 0.010;
    constexpr double THETARES  = 0.10;
    constexpr double Z0SINRES  = 1.00;
    constexpr double D0PARA    = 20.0;
    constexpr double Z0PARA    = 250.0;
    constexpr double QOPPARA   = 5.e-3;
    constexpr double Z0SINPARA = 40.0;
}

namespace IDPVMTemplates {
    const std::vector<double> populateLinearBinning(int nBins, double xMin, double xMax);
    const std::vector<double> populateLogLinearBinning(int nBins, double absXmin, double absXmax, bool symmetriseAroundZero = true);
    const std::vector<double> getResolutionBinning(IDPVMDefs::variable var); 

    TH1D getDistributionHistTemplate(IDPVMDefs::variable var, IDPVMDefs::level level);

    TH1D getEfficiencyHistTemplate(IDPVMDefs::variable var); 
    TH1D getFakeRateHistTemplate(IDPVMDefs::variable var); 

    TH2D getResolutionHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus); 
    TH2D getPullHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus);
}

#endif


