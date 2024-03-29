#ifndef __IDPVMSELECTIONS__H
#define __IDPVMSELECTIONS__H

#include "NtupleAnalysisUtils/Selection.h"
#include "IDPVMNtupleAnalysis/IDPVMTree.h"

namespace IDPVMSelections {
    Selection<IDPVMTree> dummy();

    Selection<IDPVMTree> hasTrack();
    Selection<IDPVMTree> hasTruth();
    Selection<IDPVMTree> isAssociated();

    Selection<IDPVMTree> isBestMatched();
    Selection<IDPVMTree> isTruthDuplicate();

    Selection<IDPVMTree> passedTrack();
    Selection<IDPVMTree> passedTruth();

    Selection<IDPVMTree> isSelectedByPileupSwitch();

    Selection<IDPVMTree> isMatched();
    Selection<IDPVMTree> isFake();

    Selection<IDPVMTree> isPrimary();
    Selection<IDPVMTree> isSecondary();

    Selection<IDPVMTree> forResolution();
    
    Selection<IDPVMTree> forUnlinkedRateDen();
    Selection<IDPVMTree> forUnlinkedRateNum();

    Selection<IDPVMTree> forFakeRateDen();
    Selection<IDPVMTree> forFakeRateNum();
    
    Selection<IDPVMTree> forEfficiencyDen();
    Selection<IDPVMTree> forEfficiencyNum();
}

#endif


