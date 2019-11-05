#include "IDPVMNtupleAnalysis/IDPVMSelections.h"

Selection<IDPVMTree> IDPVMSelections::hasTrack() {  
   return Selection<IDPVMTree>("hasTrack",[](IDPVMTree &t){return t.hasTrack();});
}

Selection<IDPVMTree> IDPVMSelections::hasTruth() {  
   return Selection<IDPVMTree>("hasTruth",[](IDPVMTree &t){return t.hasTruth();});
}

Selection<IDPVMTree> IDPVMSelections::isMatched() {  
   return Selection<IDPVMTree>("isMatched",[](IDPVMTree &t){return (t.track_truthMatchProb() > 0.5);});
}

Selection<IDPVMTree> IDPVMSelections::isFake() {  
   return Selection<IDPVMTree>("isFake",[](IDPVMTree &t){return t.track_isFake();});
}

Selection<IDPVMTree> IDPVMSelections::isUnassociated() {  
   return Selection<IDPVMTree>("isUnassociated",[](IDPVMTree &t){return t.truth_unassociated();});
}

Selection<IDPVMTree> IDPVMSelections::isPrimary() {  
   return Selection<IDPVMTree>("isPrimary",[](IDPVMTree &t){return (t.truth_barcode() < 200000 && t.truth_barcode() != 0);});
}

Selection<IDPVMTree> IDPVMSelections::isMatchedPrimary() {  
   return (IDPVMSelections::hasTrack() && IDPVMSelections::hasTruth() && IDPVMSelections::isMatched() && IDPVMSelections::isPrimary());
}

Selection<IDPVMTree> IDPVMSelections::forFakeNum() {  
   return (IDPVMSelections::hasTrack() && IDPVMSelections::isFake());
}

Selection<IDPVMTree> IDPVMSelections::forEfficiencyNum() {  
   return (IDPVMSelections::hasTruth() && !IDPVMSelections::isUnassociated() && IDPVMSelections::isPrimary());
}