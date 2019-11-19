#include "IDPVMNtupleAnalysis/IDPVMTemplates.h"

const std::string IDPVMLabels::getVarName(IDPVMDefs::variable var) {
   if (var == IDPVMDefs::eta) {
      return "eta";
   }
   if (var == IDPVMDefs::pt) {
      return "pt";
   }
   if (var == IDPVMDefs::d0) {
      return "d0";
   }
   if (var == IDPVMDefs::z0) {
      return "z0";
   }
   if (var == IDPVMDefs::z0sin) {
      return "z0sin";
   }
   if (var == IDPVMDefs::phi) {
      return "phi";
   }
   if (var == IDPVMDefs::theta) {
      return "theta";
   }
   if (var == IDPVMDefs::qOverPt) {
      return "qoverpt";
   }
  if (var == IDPVMDefs::qOverP) {
      return "qoverp";
   }

   return "UNNKNOWN";
}

const std::string IDPVMLabels::getResolutionLabel(IDPVMDefs::variable var) {
   if (var == IDPVMDefs::pt) {
      return "p_{T} resolution [GeV]";
   }
   if (var == IDPVMDefs::d0) {
      return "d_{0} resolution [#mum]";
   }
   if (var == IDPVMDefs::z0) {
      return "z_{0} resolution [#mum]";
   }
   if (var == IDPVMDefs::z0sin) {
      return "z_{0} #times sin(#theta) resolution [#mum]";
   }
   if (var == IDPVMDefs::phi) {
      return "#phi resolution [mrad]";
   }
   if (var == IDPVMDefs::theta) {
      return "#theta resolution [mrad]";
   }
   if (var == IDPVMDefs::qOverPt) {
      return "#sigma(q/p_{T}) / (q/p_{T}) [%]";
   }
  if (var == IDPVMDefs::qOverP) {
      return "(q/p) resolution [MeV^{-1}]";
   }

   return "UNNKNOWN";
}

const std::string IDPVMLabels::getPullWidthLabel(IDPVMDefs::variable var) {
   if (var == IDPVMDefs::pt) {
      return "p_{T} pull width";
   }
   if (var == IDPVMDefs::d0) {
      return "d_{0} pull width";
   }
   if (var == IDPVMDefs::z0) {
      return "z_{0} pull width";
   }
   if (var == IDPVMDefs::z0sin) {
      return "z_{0} #times sin(#theta) pull width";
   }
   if (var == IDPVMDefs::phi) {
      return "#phi pull width";
   }
   if (var == IDPVMDefs::theta) {
      return "#theta pull width";
   }
   if (var == IDPVMDefs::qOverPt) {
      return "(q/p_{T}) pull width";
   }
   if (var == IDPVMDefs::qOverP) {
      return "(q/p) pull width";
   }

   return "UNNKNOWN";
}

const std::string IDPVMLabels::getPullMeanLabel(IDPVMDefs::variable var) {
   if (var == IDPVMDefs::pt) {
      return "p_{T} pull mean";
   }
   if (var == IDPVMDefs::d0) {
      return "d_{0} pull mean";
   }
   if (var == IDPVMDefs::z0) {
      return "z_{0} pull mean";
   }
   if (var == IDPVMDefs::z0sin) {
      return "z_{0} #times sin(#theta) pull mean";
   }
   if (var == IDPVMDefs::phi) {
      return "#phi pull mean";
   }
   if (var == IDPVMDefs::theta) {
      return "#theta pull mean";
   }
   if (var == IDPVMDefs::qOverPt) {
      return "(q/p_{T}) pull mean";
   }
   if (var == IDPVMDefs::qOverP) {
      return "(q/p) pull mean";
   }

   return "UNNKNOWN";
}

const std::vector<double> IDPVMTemplates::populateLinearBinning(int nBins, double xMin, double xMax) {
   
   std::vector<double> theBinning; 

   double step = (xMax - xMin) / (double) nBins;

   for (int index = 0; index < nBins + 1; ++index) {
      theBinning.push_back(xMin + index * step);
   }

   return theBinning;
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
      return IDPVMTemplates::populateLinearBinning(64, -4.0, 4.0);
   }
   if (var == IDPVMDefs::pt) {
      return IDPVMTemplates::populateLogLinearBinning(49, 0.745, 1000.0, false);
   }

    return {};
}

TH2D IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus) {
   std::vector<double> vsBins = IDPVMTemplates::getResolutionBinning(versus);
   if (var == IDPVMDefs::pt) {
      std::vector<double> ptBins = IDPVMTemplates::populateLogLinearBinning(2000, 1e-3, 1000.0, true);
      if (versus == IDPVMDefs::eta) {
         return TH2D("ptResolution_vs_eta", ";truth #eta;p_{T}^{track} - p_{T}^{truth} [GeV]", vsBins.size() - 1, &(vsBins[0]), ptBins.size() - 1, &(ptBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("ptResolution_vs_pt", ";truth p_{T} [GeV];p_{T}^{track} - p_{T}^{truth} [GeV]", vsBins.size() - 1, &(vsBins[0]), ptBins.size() - 1, &(ptBins[0]));
      }
   }
   if (var == IDPVMDefs::d0) {
      std::vector<double> d0Bins = IDPVMTemplates::populateLogLinearBinning(1000, 3e-4, 30.0, true);
      if (versus == IDPVMDefs::eta) {
         return TH2D("d0Resolution_vs_eta", ";truth #eta;d_{0}^{track} - d_{0}^{truth} [mm]", vsBins.size() - 1, &(vsBins[0]), d0Bins.size() - 1, &(d0Bins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("d0Resolution_vs_pt", ";truth p_{T} [GeV];d_{0}^{track} - d_{0}^{truth} [mm]", vsBins.size() - 1, &(vsBins[0]), d0Bins.size() - 1, &(d0Bins[0]));
      }
   }
   if (var == IDPVMDefs::z0) {
      std::vector<double> z0Bins = IDPVMTemplates::populateLogLinearBinning(1000, 1e-3, 200.0, true);
      if (versus == IDPVMDefs::eta) {
         return TH2D("z0Resolution_vs_eta", ";truth #eta;z_{0}^{track} - z_{0}^{truth} [mm]", vsBins.size() - 1, &(vsBins[0]), z0Bins.size() - 1, &(z0Bins[0]));
      }
       if (versus == IDPVMDefs::pt) {
         return TH2D("z0Resolution_vs_pt", ";truth p_{T} [GeV];z_{0}^{track} - z_{0}^{truth} [mm]", vsBins.size() - 1, &(vsBins[0]), z0Bins.size() - 1, &(z0Bins[0]));
      }
   }
   if (var == IDPVMDefs::z0sin) {
      std::vector<double> z0sinBins = IDPVMTemplates::populateLogLinearBinning(1000, 5e-4, 5.0, true);
      if (versus == IDPVMDefs::eta) {
         return TH2D("z0sinResolution_vs_eta", ";truth #eta;z_{0} #times sin(#theta)^{track} - z_{0} #times sin(#theta)^{truth} [mm]", vsBins.size() - 1, &(vsBins[0]), z0sinBins.size() - 1, &(z0sinBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("z0sinResolution_vs_pt", ";truth p_{T} [GeV];z_{0} #times sin(#theta)^{track} - z_{0} #times sin(#theta)^{truth} [mm]", vsBins.size() - 1, &(vsBins[0]), z0sinBins.size() - 1, &(z0sinBins[0]));
      }
   }
   if (var == IDPVMDefs::phi) {
      std::vector<double> phiBins = IDPVMTemplates::populateLogLinearBinning(1000, 1e-5, 1e-1, true);
      if (versus == IDPVMDefs::eta) {
         return TH2D("phiResolution_vs_eta", ";truth #eta;#phi^{track} - #phi^{truth} [rad]", vsBins.size() - 1, &(vsBins[0]), phiBins.size() - 1, &(phiBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("phiResolution_vs_pt", ";truth p_{T} [GeV];#phi^{track} - #phi^{truth} [rad]", vsBins.size() - 1, &(vsBins[0]), phiBins.size() - 1, &(phiBins[0]));
      }
   }
   if (var == IDPVMDefs::theta) {
      std::vector<double> thetaBins = IDPVMTemplates::populateLogLinearBinning(2000, 1e-7, 1e-1, true);
      if (versus == IDPVMDefs::eta) {
         return TH2D("thetaResolution_vs_eta", ";truth #eta;#theta^{track} - #theta^{truth} [rad]", vsBins.size() - 1, &(vsBins[0]), thetaBins.size() - 1, &(thetaBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("thetaResolution_vs_pt", ";truth p_{T} [GeV];#theta^{track} - #theta^{truth} [rad]", vsBins.size() - 1, &(vsBins[0]), thetaBins.size() - 1, &(thetaBins[0]));
      }
   }
   if (var == IDPVMDefs::qOverPt) {
      std::vector<double> qOverPtBins = IDPVMTemplates::populateLogLinearBinning(2000, 1e-5, 50.0, true);
      if (versus == IDPVMDefs::eta) {
         return TH2D("qOverPtResolution_vs_eta", ";truth #eta;((q/p_{T})^{track} - (q/p_{T})^{truth})/(q/p_{T})^{truth}", vsBins.size() - 1, &(vsBins[0]), qOverPtBins.size() - 1, &(qOverPtBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("qOverPtResolution_vs_pt", ";truth p_{T} [GeV];((q/p_{T})^{track} - (q/p_{T})^{truth})/(q/p_{T})^{truth}", vsBins.size() - 1, &(vsBins[0]), qOverPtBins.size() - 1, &(qOverPtBins[0]));
      }
   }
   if (var == IDPVMDefs::qOverP) {
      std::vector<double> qOverPBins = IDPVMTemplates::populateLogLinearBinning(2000, 1e-7, 1e-2, true);
      if (versus == IDPVMDefs::eta) {
         return TH2D("qOverPResolution_vs_eta", ";truth #eta;(q/p)^{track} - (q/p)^{truth} [MeV^{-1}]", vsBins.size() - 1, &(vsBins[0]), qOverPBins.size() - 1, &(qOverPBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("qOverPResolution_vs_pt", ";truth p_{T} [GeV];(q/p)^{track} - (q/p)^{truth} [MeV^{-1}]", vsBins.size() - 1, &(vsBins[0]), qOverPBins.size() - 1, &(qOverPBins[0]));
      }
   }

   return TH2D();
}

TH2D IDPVMTemplates::getPullHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus) {
   std::vector<double> vsBins = IDPVMTemplates::getResolutionBinning(versus);
   std::vector<double> pullBins = IDPVMTemplates::populateLogLinearBinning(1000, 2e-2, 100.0, true);
   if (var == IDPVMDefs::pt) {
      if (versus == IDPVMDefs::eta) {
         return TH2D("ptPull_vs_eta", ";truth #eta;(p_{T}^{track} - p_{T}^{truth})/#sigma(p_{T})", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("ptPull_vs_pt", ";truth p_{T} [GeV];(p_{T}^{track} - p_{T}^{truth})/#sigma(p_{T})", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::d0) {
      if (versus == IDPVMDefs::eta) {
         return TH2D("d0Pull_vs_eta", ";truth #eta;(d_{0}^{track} - d_{0}^{truth})/#sigma(d_{0})", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("d0Pull_vs_pt", ";truth p_{T} [GeV];(d_{0}^{track} - d_{0}^{truth})/#sigma(d_{0})", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::z0) {
      if (versus == IDPVMDefs::eta) {
         return TH2D("z0Pull_vs_eta", ";truth #eta;(z_{0}^{track} - z_{0}^{truth})/#sigma(z_{0})", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("z0Pull_vs_pt", ";truth p_{T} [GeV];(z_{0}^{track} - z_{0}^{truth})/#sigma(z_{0})", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::z0sin) {
      if (versus == IDPVMDefs::eta) {
         return TH2D("z0sinPull_vs_eta", ";truth #eta;(z_{0} #times sin(#theta)^{track} - z_{0} #times sin(#theta)^{truth})/#sigma(z_{0} #times sin(#theta))", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("z0sinPull_vs_pt", ";truth p_{T} [GeV];(z_{0} #times sin(#theta)^{track} - z_{0} #times sin(#theta)^{truth})/#sigma(z_{0} #times sin(#theta))", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::phi) {
      if (versus == IDPVMDefs::eta) {
         return TH2D("phiPull_vs_eta", ";truth #eta;(#phi^{track} - #phi^{truth})/#sigma(#phi)", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("phiPull_vs_pt", ";truth p_{T} [GeV];(#phi^{track} - #phi^{truth})/#sigma(#phi)", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::theta) {
      if (versus == IDPVMDefs::eta) {
         return TH2D("thetaPull_vs_eta", ";truth #eta;(#theta^{track} - #theta^{truth})/#sigma(#theta)", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("thetaPull_vs_pt", ";truth p_{T} [GeV];(#theta^{track} - #theta^{truth})/#sigma(#theta)", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::qOverPt) {
      if (versus == IDPVMDefs::eta) {
         return TH2D("qOverPtPull_vs_eta", ";truth #eta;((q/p_{T})^{track} - (q/p_{T})^{truth})/#sigma(q/p_{T})", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("qOverPtPull_vs_pt", ";truth p_{T} [GeV];((q/p_{T})^{track} - (q/p_{T})^{truth})/#sigma(q/p_{T})", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::qOverP) {
      if (versus == IDPVMDefs::eta) {
         return TH2D("qOverPPull_vs_eta", ";truth #eta;((q/p)^{track} - (q/p)^{truth})/#sigma(q/p)", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
      }
      if (versus == IDPVMDefs::pt) {
         return TH2D("qOverPPull_vs_pt", ";truth p_{T} [GeV];((q/p)^{track} - (q/p)^{truth})/#sigma(q/p)", vsBins.size() - 1, &(vsBins[0]), pullBins.size() - 1, &(pullBins[0]));
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
      return TH1D("etaFakeRate", ";track #eta;Fake Rate", 20, -4.0, 4.0);
   }
   if (var == IDPVMDefs::pt) {
      return TH1D("ptFakeRate", ";track p_{T} [GeV];Fake Rate", 24, 0, 50);
   }
   
   return TH1D();
}
