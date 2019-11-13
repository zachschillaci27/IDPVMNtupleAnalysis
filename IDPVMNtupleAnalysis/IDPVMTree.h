#ifndef NTAU__IDPVMTREE__H
#define NTAU__IDPVMTREE__H

#include "NtupleAnalysisUtils/NtupleBranch.h"
#include "TTree.h"
#include <vector>

class IDPVMTree: public NtupleBranchMgr{
    public:
        IDPVMTree(TTree* t);
        /// List of branch members
        NtupleBranch <Int_t>                        hasTrack;
        NtupleBranch <Int_t>                        hasTruth;
        NtupleBranch <Int_t>                        passedTrackSelection;
        NtupleBranch <Int_t>                        passedTruthSelection;
        NtupleBranch <Double_t>                     trackErr_d0;
        NtupleBranch <Double_t>                     trackErr_phi;
        NtupleBranch <Double_t>                     trackErr_qOverP;
        NtupleBranch <Double_t>                     trackErr_qOverPt;
        NtupleBranch <Double_t>                     trackErr_pt;
        NtupleBranch <Double_t>                     trackErr_theta;
        NtupleBranch <Double_t>                     trackErr_z0;
        NtupleBranch <Double_t>                     trackErr_z0sin;
        NtupleBranch <Double_t>                     track_charge;
        NtupleBranch <Double_t>                     track_d0;
        NtupleBranch <Double_t>                     track_eta;
        NtupleBranch <Double_t>                     track_phi;
        NtupleBranch <Double_t>                     track_pt;
        NtupleBranch <Double_t>                     track_qOverP;
        NtupleBranch <Double_t>                     track_qOverPt;
        NtupleBranch <Double_t>                     track_theta;
        NtupleBranch <Double_t>                     track_truthMatchProb;
        NtupleBranch <Double_t>                     track_z0;
        NtupleBranch <Double_t>                     track_z0sin;
        NtupleBranch <Int_t>                        truth_selectedByPileupSwitch;
        NtupleBranch <Int_t>                        truth_barcode;
        NtupleBranch <Int_t>                        truth_pdgId;
        NtupleBranch <Double_t>                     truth_charge;
        NtupleBranch <Double_t>                     truth_d0;
        NtupleBranch <Double_t>                     truth_eta;
        NtupleBranch <Double_t>                     truth_phi;
        NtupleBranch <Double_t>                     truth_pt;
        NtupleBranch <Double_t>                     truth_qOverP;
        NtupleBranch <Double_t>                     truth_qOverPt;
        NtupleBranch <Double_t>                     truth_theta;
        NtupleBranch <Double_t>                     truth_z0;
        NtupleBranch <Double_t>                     truth_z0sin;
};
#endif // NTAU__IDPVMTREE__H
