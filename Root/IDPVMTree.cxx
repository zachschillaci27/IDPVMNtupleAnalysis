#include "IDPVMNtupleAnalysis/IDPVMTree.h"

IDPVMTree::IDPVMTree( TTree* t): NtupleBranchMgr(t),
    acceptedAssocTruth    ("acceptedAssocTruth",     t, this),
    hasTrack              ("hasTrack",               t, this),
    hasTruth              ("hasTruth",               t, this),
    trackErr_d0           ("trackErr_d0",            t, this),
    trackErr_phi          ("trackErr_phi",           t, this),
    trackErr_qOverP       ("trackErr_qOverP",        t, this),
    trackErr_qOverPt      ("trackErr_qOverPt",       t, this),
    trackErr_theta        ("trackErr_theta",         t, this),
    trackErr_z0           ("trackErr_z0",            t, this),
    trackErr_z0Sin        ("trackErr_z0Sin",         t, this),
    track_charge          ("track_charge",           t, this),
    track_d0              ("track_d0",               t, this),
    track_eta             ("track_eta",              t, this),
    track_isFake          ("track_isFake",           t, this),
    track_phi             ("track_phi",              t, this),
    track_pt              ("track_pt",               t, this),
    track_qOverP          ("track_qOverP",           t, this),
    track_qOverPt         ("track_qOverPt",          t, this),
    track_theta           ("track_theta",            t, this),
    track_truthMatchProb  ("track_truthMatchProb",   t, this),
    track_z0              ("track_z0",               t, this),
    track_z0Sin           ("track_z0Sin",            t, this),
    truth_barcode         ("truth_barcode",          t, this),
    truth_pdgId           ("truth_pdgId",            t, this),
    truth_charge          ("truth_charge",           t, this),
    truth_d0              ("truth_d0",               t, this),
    truth_eta             ("truth_eta",              t, this),
    truth_phi             ("truth_phi",              t, this),
    truth_pt              ("truth_pt",               t, this),
    truth_qOverP          ("truth_qOverP",           t, this),
    truth_qOverPt         ("truth_qOverPt",          t, this),
    truth_theta           ("truth_theta",            t, this),
    truth_unassociated    ("truth_unassociated",     t, this),
    truth_z0              ("truth_z0",               t, this),
    truth_z0Sin           ("truth_z0Sin",            t, this)    {
        if(t) getMissedBranches(t);
    }
