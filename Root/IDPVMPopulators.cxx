#include "IDPVMNtupleAnalysis/IDPVMPopulators.h"

PlotFillInstructionWithRef<TH2D, IDPVMTree> IDPVMPopulators::getResolutionPopulator(IDPVMDefs::variable var, IDPVMDefs::variable versus) {
   TH2D hRes = IDPVMTemplates::getResolutionHistTemplate(var, versus);
   if (var == IDPVMDefs::pt) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("ptResolution_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_pt() - t.truth_pt()) / 1000.);}, hRes);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("ptResolution_pt", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_pt() - t.truth_pt()) / 1000.);}, hRes);
      }
   }
   if (var == IDPVMDefs::d0) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("d0Resolution_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_d0() - t.truth_d0()));}, hRes);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("d0Resolution_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_d0() - t.truth_d0()));}, hRes);
      }
   }
   if (var == IDPVMDefs::z0) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("z0Resolution_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_z0() - t.truth_z0()));}, hRes);
      }
       if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("z0Resolution_pt", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_z0() - t.truth_z0()));}, hRes);
      }
   }
   if (var == IDPVMDefs::z0sin) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("z0sinResolution_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_z0sin() - t.truth_z0sin()));}, hRes);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("z0sinResolution_pt", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_z0sin() - t.truth_z0sin()));}, hRes);
      }
   }
   if (var == IDPVMDefs::phi) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("phiResolution_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_phi() - t.truth_phi()));}, hRes);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("phiResolution_pt", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_phi() - t.truth_phi()));}, hRes);
      }
   }
   if (var == IDPVMDefs::theta) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("thetaResolution_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_theta() - t.truth_theta()));}, hRes);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("thetaResolution_pt", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_theta() - t.truth_theta()));}, hRes);
      }
   }
   if (var == IDPVMDefs::qOverPt) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("qOverPtResolution_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_qOverPt() - t.truth_qOverPt()) / t.truth_qOverPt());}, hRes);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("qOverPtResolution_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_qOverPt() - t.truth_qOverPt()) / t.truth_qOverPt());}, hRes);
      }
   }
   if (var == IDPVMDefs::qOverP) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("qOverPResolution_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_qOverP() - t.truth_qOverP()));}, hRes);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("qOverPResolution_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_qOverP() - t.truth_qOverP()));}, hRes);
      }
   }

   return PlotFillInstructionWithRef<TH2D, IDPVMTree>("EmptyResolution",[](TH2D* h, IDPVMTree &t){}, hRes);
}

PlotFillInstructionWithRef<TH2D, IDPVMTree> IDPVMPopulators::getPullPopulator(IDPVMDefs::variable var, IDPVMDefs::variable versus) {
   TH2D hPull = IDPVMTemplates::getPullHistTemplate(var, versus);
   if (var == IDPVMDefs::pt) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("ptPull_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_pt() - t.truth_pt()) / t.trackErr_pt());}, hPull);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("ptPull_pt", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_pt() - t.truth_pt()) / t.trackErr_pt());}, hPull);
      }
   }
   if (var == IDPVMDefs::d0) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("d0Pull_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_d0() - t.truth_d0()) / t.trackErr_d0());}, hPull);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("d0Pull_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_d0() - t.truth_d0()) / t.trackErr_d0());}, hPull);
      }
   }
   if (var == IDPVMDefs::z0) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("z0Pull_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_z0() - t.truth_z0()) / t.trackErr_z0());}, hPull);
      }
       if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("z0Pull_pt", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_z0() - t.truth_z0()) / t.trackErr_z0());}, hPull);
      }
   }
   if (var == IDPVMDefs::z0sin) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("z0sinPull_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_z0sin() - t.truth_z0sin()) / t.trackErr_z0sin());}, hPull);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("z0sinPull_pt", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_z0sin() - t.truth_z0sin()) / t.trackErr_z0sin());}, hPull);
      }
   }
   if (var == IDPVMDefs::phi) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("phiPull_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_phi() - t.truth_phi()) / t.trackErr_phi());}, hPull);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("phiPull_pt", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_phi() - t.truth_phi()) / t.trackErr_phi());}, hPull);
      }
   }
   if (var == IDPVMDefs::theta) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("thetaPull_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_theta() - t.truth_theta()) / t.trackErr_theta());}, hPull);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("thetaPull_pt", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_theta() - t.truth_theta()) / t.trackErr_theta());}, hPull);
      }
   }
   if (var == IDPVMDefs::qOverPt) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("qOverPtPull_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_qOverPt() - t.truth_qOverPt()) / t.trackErr_qOverPt());}, hPull);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("qOverPtPull_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_qOverPt() - t.truth_qOverPt()) / t.trackErr_qOverPt());}, hPull);
      }
   }
   if (var == IDPVMDefs::qOverP) {
      if (versus == IDPVMDefs::eta) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("qOverPPull_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_eta(), (t.track_qOverP() - t.truth_qOverP()) / t.trackErr_qOverP());}, hPull);
      }
      if (versus == IDPVMDefs::pt) {
         return PlotFillInstructionWithRef<TH2D, IDPVMTree>("qOverPPull_eta", [](TH2D* h, IDPVMTree &t){h->Fill(t.truth_pt() / 1000., (t.track_qOverP() - t.truth_qOverP()) / t.trackErr_qOverP());}, hPull);
      }
   }

   return PlotFillInstructionWithRef<TH2D, IDPVMTree>("EmptyPull",[](TH2D* h, IDPVMTree &t){}, hPull);
}