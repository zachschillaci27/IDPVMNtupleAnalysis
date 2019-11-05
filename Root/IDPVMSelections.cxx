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

Selection<IDPVMTree> IDPVMSelections::isAcceptedAssocTruth() {  
   return Selection<IDPVMTree>("isAcceptedAssocTruth",[](IDPVMTree &t){return t.acceptedAssocTruth();});
}

Selection<IDPVMTree> IDPVMSelections::isPrimary() {  
   return Selection<IDPVMTree>("isPrimary",[](IDPVMTree &t){return (t.truth_barcode() != 0 && t.truth_barcode() < 200000);});
}

Selection<IDPVMTree> IDPVMSelections::trackKinematics() {  
   return Selection<IDPVMTree>("trackKinematics",[](IDPVMTree &t){return (t.track_pt() > 1000. && fabs(t.track_eta()) < 4.0);});
}

Selection<IDPVMTree> IDPVMSelections::truthKinematics() {  
   return Selection<IDPVMTree>("truthKinematics",[](IDPVMTree &t){return (t.truth_pt() > 1000. && fabs(t.truth_eta()) < 4.0);});
}

Selection<IDPVMTree> IDPVMSelections::forResolution() {  
   return (IDPVMSelections::trackKinematics() && IDPVMSelections::truthKinematics() &&
           IDPVMSelections::isMatched() && IDPVMSelections::isPrimary() && IDPVMSelections::isAcceptedAssocTruth());
}

Selection<IDPVMTree> IDPVMSelections::forFakeRateNum() {  
   return (IDPVMSelections::trackKinematics() && IDPVMSelections::isFake());
}

Selection<IDPVMTree> IDPVMSelections::forFakeRateDen() {  
   return (IDPVMSelections::trackKinematics());
}

Selection<IDPVMTree> IDPVMSelections::forEfficiencyNum() {  
   return (IDPVMSelections::truthKinematics() && !IDPVMSelections::isUnassociated() && IDPVMSelections::isMatched());
}

Selection<IDPVMTree> IDPVMSelections::forEfficiencyDen() {  
   return (IDPVMSelections::truthKinematics());
}