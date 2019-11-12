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
      return {-4.000,-3.875,-3.750,-3.625,-3.500,-3.375,-3.250,-3.125,-3.000,-2.875,-2.750,-2.625,-2.500,-2.375,-2.250,-2.125,-2.000,-1.875,-1.750,-1.625,-1.500,-1.375,-1.250,-1.125,-1.000,-0.875,-0.750,-0.625,-0.500,-0.375,-0.250,-0.125,0.000,0.125,0.250,0.375,0.500,0.625,0.750,0.875,1.000,1.125,1.250,1.375,1.500,1.625,1.750,1.875,2.000,2.125,2.250,2.375,2.500,2.625,2.750,2.875,3.000,3.125,3.250,3.375,3.500,3.625,3.750,3.875,4.000};
   }

    return {};
}

TH2D IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus) {
   std::vector<double> vsBins = IDPVMTemplates::getResolutionBinning(versus);
   if (var == IDPVMDefs::d0) {
      std::vector<double> d0Bins = IDPVMTemplates::populateLogLinearBinning(1000, 3e-4, 30, true);
      if (versus == IDPVMDefs::eta) {
         return TH2D("d0Resolution", ";truth #eta;d_{0}^{track} - d_{0}^{truth} [mm]", vsBins.size() - 1, &(vsBins[0]), d0Bins.size() - 1, &(d0Bins[0]));
      }
   }
   if (var == IDPVMDefs::z0) {
      std::vector<double> z0Bins = IDPVMTemplates::populateLogLinearBinning(1000, 1e-3, 200.0, true);
      if (versus == IDPVMDefs::eta) {
         return TH2D("z0Resolution", ";truth #eta;z_{0}^{track} - z_{0}^{truth} [mm]", vsBins.size() - 1, &(vsBins[0]), z0Bins.size() - 1, &(z0Bins[0]));
      }
   }
   if (var == IDPVMDefs::phi) {
      std::vector<double> phiBins = IDPVMTemplates::populateLogLinearBinning(1000, 1e-5, 1e-1, true);
      if (versus == IDPVMDefs::eta) {
         return TH2D("phiResolution", ";truth #eta;#phi^{track} - #phi^{truth} [rad]", vsBins.size() - 1, &(vsBins[0]), phiBins.size() - 1, &(phiBins[0]));
      }
   }
   if (var == IDPVMDefs::theta) {
      std::vector<double> thetaBins = IDPVMTemplates::populateLogLinearBinning(2000, 1e-7, 1e-1, true);
      if (versus == IDPVMDefs::eta) {
         return TH2D("thetaResolution", ";truth #eta;#theta^{track} - #theta^{truth} [rad]", vsBins.size() - 1, &(vsBins[0]), thetaBins.size() - 1, &(thetaBins[0]));
      }
   }
   if (var == IDPVMDefs::qOverPt) {
      std::vector<double> qOverPtBins = IDPVMTemplates::populateLogLinearBinning(2000, 1e-5, 50, true);
      if (versus == IDPVMDefs::eta) {
         return TH2D("qOverPtResolution", ";truth #eta;#(q/p_{T})^{track} - #(q/p_{T})^{truth} [MeV^{-1}]", vsBins.size() - 1, &(vsBins[0]), qOverPtBins.size() - 1, &(qOverPtBins[0]));
      }
   }

   return TH2D();
}

TH2D IDPVMTemplates::getPullHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus) {
   std::vector<double> vsBins = IDPVMTemplates::getResolutionBinning(versus);
   std::vector<double> pullBins = IDPVMTemplates::populateLogLinearBinning(1000, 2e-2, 100, true);
   if (var == IDPVMDefs::d0) {
      if (versus == IDPVMDefs::eta) {
         return TH2D("d0Pull", ";truth #eta;(d_{0}^{track} - d_{0}^{truth})/#sigma_{d_{0}}", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::z0) {
      if (versus == IDPVMDefs::eta) {
         return TH2D("z0Pull", ";truth #eta;(z_{0}^{track} - z_{0}^{truth})/#sigma_{z_{0}}", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::phi) {
      if (versus == IDPVMDefs::eta) {
         return TH2D("phiPull", ";truth #eta;(#phi^{track} - #phi^{truth})/#sigma_{#phi}", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::theta) {
      if (versus == IDPVMDefs::eta) {
         return TH2D("thetaPull", ";truth #eta;(#theta^{track} - #theta^{truth})/#sigma_{#theta}", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::qOverPt) {
      if (versus == IDPVMDefs::eta) {
         return TH2D("qOverPtPull", ";truth #eta;#((q/p_{T})^{track} - #(q/p_{T})^{truth})/#sigma_{q/p_{T}}", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }

   return TH2D();
}

TH1D IDPVMTemplates::getEfficiencyHistTemplate(IDPVMDefs::variable var) {
   if (var == IDPVMDefs::eta) {
      return TH1D("etaEfficiency", ";truth #eta;Efficiency", 20, -4.0, 4.0);
   }
   if (var == IDPVMDefs::pt) {
      return TH1D("ptEfficiency", ";truth p_{T} [GeV];Efficiency", 25, 0, 50);
   }
   
   return TH1D();
}

TH1D IDPVMTemplates::getFakeRateHistTemplate(IDPVMDefs::variable var) {
   if (var == IDPVMDefs::eta) {
      return TH1D("etaeFakeRate", ";track #eta;Fake Rate", 10, -4.0, 4.0);
   }
   if (var == IDPVMDefs::pt) {
      return TH1D("pteFakeRate", ";track p_{T} [GeV];Fake Rate", 10, 0, 50);
   }
   
   return TH1D();
}