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
    Selection<IDPVMTree> isPrimary();

    Selection<IDPVMTree> isMatchedPrimary();

    Selection<IDPVMTree> forFakeNum();
    Selection<IDPVMTree> forEfficiencyNum();
}

#endif


