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

const std::vector<double> IDPVMTemplates::get1DBinning(IDPVMDefs::variable var, IDPVMDefs::plot plt) {
   if (var == IDPVMDefs::phi) {
      return {-3.141593,-3.078761,-3.015929,-2.953097,-2.890265,-2.827433,-2.764601,-2.701770,-2.638938,-2.576106,-2.513274,-2.450442,-2.387610,-2.324778,-2.261947,-2.199115,-2.136283,-2.073451,-2.010619,-1.947787,-1.884956,-1.822124,-1.759292,-1.696460,-1.633628,-1.570796,-1.507964,-1.445133,-1.382301,-1.319469,-1.256637,-1.193805,-1.130973,-1.068141,-1.005310,-0.942478,-0.879646,-0.816814,-0.753982,-0.691150,-0.628319,-0.565487,-0.502655,-0.439823,-0.376991,-0.314159,-0.251327,-0.188496,-0.125664,-0.062832,0.000000,0.062832,0.125664,0.188496,0.251327,0.314159,0.376991,0.439823,0.502655,0.565487,0.628319,0.691150,0.753982,0.816814,0.879646,0.942478,1.005310,1.068141,1.130973,1.193805,1.256637,1.319469,1.382301,1.445133,1.507964,1.570796,1.633628,1.696460,1.759292,1.822124,1.884956,1.947787,2.010619,2.073451,2.136283,2.199115,2.261947,2.324778,2.387610,2.450442,2.513274,2.576106,2.638938,2.701770,2.764601,2.827433,2.890265,2.953097,3.015929,3.141593};
   }
   if (var == IDPVMDefs::theta) {
      return {0.000000,0.031416,0.062832,0.094248,0.125664,0.157080,0.188496,0.219911,0.251327,0.282743,0.314159,0.345575,0.376991,0.408407,0.439823,0.471239,0.502655,0.534071,0.565487,0.596903,0.628319,0.659734,0.691150,0.722566,0.753982,0.785398,0.816814,0.848230,0.879646,0.911062,0.942478,0.973894,1.005310,1.036726,1.068141,1.099557,1.130973,1.162389,1.193805,1.225221,1.256637,1.288053,1.319469,1.350885,1.382301,1.413717,1.445133,1.476548,1.507964,1.539380,1.570796,1.602212,1.633628,1.665044,1.696460,1.727876,1.759292,1.790708,1.822124,1.853540,1.884956,1.916371,1.947787,1.979203,2.010619,2.042035,2.073451,2.104867,2.136283,2.167699,2.199115,2.230531,2.261947,2.293363,2.324778,2.356194,2.387610,2.419026,2.450442,2.481858,2.513274,2.544690,2.576106,2.607522,2.638938,2.670354,2.701770,2.733185,2.764601,2.796017,2.827433,2.858849,2.890265,2.921681,2.953097,2.984513,3.015929,3.047345,3.078761,3.141593};
   }
   if (var == IDPVMDefs::eta) {
      if (plt == IDPVMDefs::resolution) {
         return {-4.000000,-3.875000,-3.750000,-3.625000,-3.500000,-3.375000,-3.250000,-3.125000,-3.000000,-2.875000,-2.750000,-2.625000,-2.500000,-2.375000,-2.250000,-2.125000,-2.000000,-1.875000,-1.750000,-1.625000,-1.500000,-1.375000,-1.250000,-1.125000,-1.000000,-0.875000,-0.750000,-0.625000,-0.500000,-0.375000,-0.250000,-0.125000,0.000000,0.125000,0.250000,0.375000,0.500000,0.625000,0.750000,0.875000,1.000000,1.125000,1.250000,1.375000,1.500000,1.625000,1.750000,1.875000,2.000000,2.125000,2.250000,2.375000,2.500000,2.625000,2.750000,2.875000,3.000000,3.125000,3.250000,3.375000,3.500000,3.625000,3.750000,4.000000};
      }
      if (plt == IDPVMDefs::efficiency) {
         return {};
      }
   }

    return {};
}

TH2D IDPVMTemplates::getResolutionHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus) {
   if (var == IDPVMDefs::d0) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> resBins = IDPVMTemplates::populateLogLinearBinning(1000, 3e-4, 30);
         return TH2D("d0Resolution", ";#eta;d_{0}^{track} - d_{0}^{truth} [mm]", 64, -5.0, 5.0, resBins.size() - 1, &(resBins[0]));
      }
   }
   if (var == IDPVMDefs::z0) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> resBins = IDPVMTemplates::populateLogLinearBinning(1000, 1e-3, 200.0);
         return TH2D("z0Resolution", ";#eta;z_{0}^{track} - z_{0}^{truth} [mm]", 64, -5.0, 5.0, resBins.size() - 1, &(resBins[0]));
      }
   }
   if (var == IDPVMDefs::phi) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> resBins = IDPVMTemplates::populateLogLinearBinning(1000, 1e-5, 1e-1);
         return TH2D("phiResolution", ";#eta;#phi^{track} - #phi^{truth} [rad]", 64, -5.0, 5.0, resBins.size() - 1, &(resBins[0]));
      }
   }
   if (var == IDPVMDefs::theta) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> resBins = IDPVMTemplates::populateLogLinearBinning(2000, 1e-7, 1e-1);
         return TH2D("thetaResolution", ";#eta;#theta^{track} - #theta^{truth} [rad]", 64, -5.0, 5.0, resBins.size() - 1, &(resBins[0]));
      }
   }
   if (var == IDPVMDefs::qOverPt) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> resBins = IDPVMTemplates::populateLogLinearBinning(2000, 1e-5, 50);
         return TH2D("qOverPtResolution", ";#eta;#(q/p_{T})^{track} - #(q/p_{T})^{truth} [MeV^{-1}]", 64, -5.0, 5.0, resBins.size() - 1, &(resBins[0]));
      }
   }

   return TH2D();
}

TH2D IDPVMTemplates::getPullHistTemplate(IDPVMDefs::variable var, IDPVMDefs::variable versus) {
   if (var == IDPVMDefs::d0) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> pullBins = IDPVMTemplates::populateLogLinearBinning(1000, 2e-2, 100);
         return TH2D("d0Pull", ";#eta;(d_{0}^{track} - d_{0}^{truth})/#sigma_{d_{0}}", 50, -5.0, 5.0, pullBins.size() - 1, &(pullBins[0]));
      }
   }
   if (var == IDPVMDefs::z0) {
      if (versus == IDPVMDefs::eta) {
         std::vector<double> pullBins = IDPVMTemplates::populateLogLinearBinning(1000, 2e-2, 100);
         return TH2D("d0Pull", ";#eta;(z_{0}^{track} - z_{0}^{truth})/#sigma_{z_{0}}", 50, -5.0, 5.0, pullBins.size() - 1, &(pullBins[0]));
      }
   }

   return TH2D();
}