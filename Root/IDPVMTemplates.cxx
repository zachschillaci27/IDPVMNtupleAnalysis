#include "IDPVMNtupleAnalysis/IDPVMTemplates.h"

const std::string IDPVMLabels::getResolutionLabel(IDPVMDefs::variable var) {
   if (var == IDPVMDefs::d0) {
      return "d_{0} resolution [#mum]";
   }
   if (var == IDPVMDefs::z0) {
      return "z_{0} resolution [#mum]";
   }
   if (var == IDPVMDefs::phi) {
      return "#phi resolution [mrad]";
   }
   if (var == IDPVMDefs::theta) {
      return "#theta resolution [mrad]";
   }
   if (var == IDPVMDefs::qOverPt) {
      return "p_{T}/q #times #sigma(q/p_{T})";
   }

   return "";
}

const std::string IDPVMLabels::getPullWidthLabel(IDPVMDefs::variable var) {
   if (var == IDPVMDefs::d0) {
      return "d_{0} pull width";
   }
   if (var == IDPVMDefs::z0) {
      return "z_{0} pull width";
   }
   if (var == IDPVMDefs::phi) {
      return "#phi pull width";
   }
   if (var == IDPVMDefs::theta) {
      return "#theta pull width";
   }
   if (var == IDPVMDefs::qOverPt) {
      return "(1/p_{T}) pull width";
   }

   return "";
}

const std::string IDPVMLabels::getPullMeanLabel(IDPVMDefs::variable var) {
   if (var == IDPVMDefs::d0) {
      return "d_{0} pull mean";
   }
   if (var == IDPVMDefs::z0) {
      return "z_{0} pull mean";
   }
   if (var == IDPVMDefs::phi) {
      return "#phi pull mean";
   }
   if (var == IDPVMDefs::theta) {
      return "#theta pull mean";
   }
   if (var == IDPVMDefs::qOverPt) {
      return "(1/p_{T}) pull mean";
   }

   return "";
}

const std::vector<double> IDPVMTemplates::populateLogLinearBinning(int nBins, double absXmin, double absXmax, bool symmetriseAroundZero) {

   std::vector<double> theBinning; 

   // reserve some space
   if (symmetriseAroundZero) {
      theBinning.reserve(2 * nBins + 2);
   }
   else {
      theBinning.reserve(nBins + 1);
   }

   // define our starting bin edge and step size in log space
   double logStart = log(absXmin);
   double logdist = log(absXmax) - logStart;
   double logstep = logdist / (double) nBins;

   // then populate the bin array
   for (int index = 0; index < nBins + 1; ++index) {
      theBinning.push_back(exp(logStart + index * logstep));
   }
   // if we want to symmetrise, we need one extra step to add the negative 
   // half axis (and the division at zero). 
   if (symmetriseAroundZero) {
      std::vector<double> aux_negative;
      aux_negative.reserve(nBins + 1);
      // flip signs (and populate new vec)
      std::transform(theBinning.begin(), theBinning.end(), std::back_inserter(aux_negative), [](double & val){return -1. * val;});
      // reorder
      std::reverse(aux_negative.begin(), aux_negative.end());
      // and add the split at zero 
      aux_negative.push_back(0.);
      // then put it all together
      theBinning.insert(theBinning.begin(), aux_negative.begin(), aux_negative.end()); 
   }

  return theBinning;
}

const std::vector<double> IDPVMTemplates::getResolutionBinning(IDPVMDefs::variable var) {
   if (var == IDPVMDefs::eta) {
      return {-4.000000,-3.875000,-3.750000,-3.625000,-3.500000,-3.375000,-3.250000,-3.125000,-3.000000,-2.875000,-2.750000,-2.625000,-2.500000,-2.375000,-2.250000,-2.125000,-2.000000,-1.875000,-1.750000,-1.625000,-1.500000,-1.375000,-1.250000,-1.125000,-1.000000,-0.875000,-0.750000,-0.625000,-0.500000,-0.375000,-0.250000,-0.125000,0.000000,0.125000,0.250000,0.375000,0.500000,0.625000,0.750000,0.875000,1.000000,1.125000,1.250000,1.375000,1.500000,1.625000,1.750000,1.875000,2.000000,2.125000,2.250000,2.375000,2.500000,2.625000,2.750000,2.875000,3.000000,3.125000,3.250000,3.375000,3.500000,3.625000,3.750000,4.000000};
   }

    return {};
}

TH2D IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus) {
   std::vector<double> vsBins = IDPVMTemplates::getResolutionBinning(versus);
   if (var == IDPVMDefs::d0) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> resBins = IDPVMTemplates::populateLogLinearBinning(1000, 3e-4, 30);
         return TH2D("d0Resolution", ";#eta;d_{0}^{track} - d_{0}^{truth} [mm]", vsBins.size() - 1, &(vsBins[0]), resBins.size() - 1, &(resBins[0]));
      }
   }
   if (var == IDPVMDefs::z0) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> resBins = IDPVMTemplates::populateLogLinearBinning(1000, 1e-3, 200.0);
         return TH2D("z0Resolution", ";#eta;z_{0}^{track} - z_{0}^{truth} [mm]", vsBins.size() - 1, &(vsBins[0]), resBins.size() - 1, &(resBins[0]));
      }
   }
   if (var == IDPVMDefs::phi) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> resBins = IDPVMTemplates::populateLogLinearBinning(1000, 1e-5, 1e-1);
         return TH2D("phiResolution", ";#eta;#phi^{track} - #phi^{truth} [rad]", vsBins.size() - 1, &(vsBins[0]), resBins.size() - 1, &(resBins[0]));
      }
   }
   if (var == IDPVMDefs::theta) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> resBins = IDPVMTemplates::populateLogLinearBinning(2000, 1e-7, 1e-1);
         return TH2D("thetaResolution", ";#eta;#theta^{track} - #theta^{truth} [rad]", vsBins.size() - 1, &(vsBins[0]), resBins.size() - 1, &(resBins[0]));
      }
   }
   if (var == IDPVMDefs::qOverPt) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> resBins = IDPVMTemplates::populateLogLinearBinning(2000, 1e-5, 50);
         return TH2D("qOverPtResolution", ";#eta;#(q/p_{T})^{track} - #(q/p_{T})^{truth} [MeV^{-1}]", vsBins.size() - 1, &(vsBins[0]), resBins.size() - 1, &(resBins[0]));
      }
   }

   return TH2D();
}

TH2D IDPVMTemplates::getPullHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus) {
   std::vector<double> vsBins = IDPVMTemplates::getResolutionBinning(versus);
   if (var == IDPVMDefs::d0) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> pullBins = IDPVMTemplates::populateLogLinearBinning(1000, 2e-2, 100);
         return TH2D("d0Pull", ";#eta;(d_{0}^{track} - d_{0}^{truth})/#sigma_{d_{0}}", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::z0) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> pullBins = IDPVMTemplates::populateLogLinearBinning(1000, 2e-2, 100);
         return TH2D("z0Pull", ";#eta;(z_{0}^{track} - z_{0}^{truth})/#sigma_{z_{0}}", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::phi) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> pullBins = IDPVMTemplates::populateLogLinearBinning(1000, 2e-2, 100);
         return TH2D("phiPull", ";#eta;(#phi^{track} - #phi^{truth})/#sigma_{#phi}", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::theta) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> pullBins = IDPVMTemplates::populateLogLinearBinning(1000, 2e-2, 100);
         return TH2D("thetaPull", ";#eta;(#theta^{track} - #theta^{truth})/#sigma_{#theta}", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }

   return TH2D();
}