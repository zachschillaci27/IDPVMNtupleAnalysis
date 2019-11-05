#ifndef __IDPVMSELECTIONS__H
#define __IDPVMSELECTIONS__H

#include "NtupleAnalysisUtils/Selection.h"
#include "IDPVMNtupleAnalysis/IDPVMTree.h"

namespace IDPVMSelections {
    Selection<IDPVMTree> hasTrack();
    Selection<IDPVMTree> hasTruth();
    
    Selection<IDPVMTree> isMatched();
    Selection<IDPVMTree> isFake();
    Selection<IDPVMTree> isUnassociated();
    Selection<IDPVMTree> isAcceptedAssocTruth();
    Selection<IDPVMTree> isPrimary();

    Selection<IDPVMTree> trackKinematics();
    Selection<IDPVMTree> truthKinematics();

    Selection<IDPVMTree> forResolution();
    Selection<IDPVMTree> forFakeRateNum();
    Selection<IDPVMTree> forFakeRateDen();
    Selection<IDPVMTree> forEfficiencyNum();
    Selection<IDPVMTree> forEfficiencyDen();
}

#endif


