#include "IDPVMNtupleAnalysis/IDPVMSelections.h"

Selection<IDPVMTree> IDPVMSelections::hasTrack() {  
   return Selection<IDPVMTree>("hasTrack",[](IDPVMTree &t){return t.hasTrack();});
}

Selection<IDPVMTree> IDPVMSelections::hasTruth() {  
   return Selection<IDPVMTree>("hasTruth",[](IDPVMTree &t){return t.hasTruth();});
}

Selection<IDPVMTree> IDPVMSelections::passedTrack() {  
   return Selection<IDPVMTree>("passedTrack",[](IDPVMTree &t){return t.passedTrackSelection();});
}

Selection<IDPVMTree> IDPVMSelections::passedTruth() {  
   return Selection<IDPVMTree>("passedTruth",[](IDPVMTree &t){return t.passedTruthSelection();});
}

Selection<IDPVMTree> IDPVMSelections::isMatched() {  
   return Selection<IDPVMTree>("isMatched",[](IDPVMTree &t){return (t.track_truthMatchProb() > 0.5);});
}

Selection<IDPVMTree> IDPVMSelections::isFake() {  
   return Selection<IDPVMTree>("isFake",[](IDPVMTree &t){return (t.track_truthMatchProb() < 0.5);});
}

Selection<IDPVMTree> IDPVMSelections::isAssociated() {  
   return Selection<IDPVMTree>("isAssociated",[](IDPVMTree &t){return t.truth_associated();});
}

Selection<IDPVMTree> IDPVMSelections::isSelectedByPileupSwitch() {  
   return Selection<IDPVMTree>("isSelectedByPileupSwitch",[](IDPVMTree &t){return t.truth_selectedByPileupSwitch();});
}

Selection<IDPVMTree> IDPVMSelections::isPrimary() {  
   return Selection<IDPVMTree>("isPrimary",[](IDPVMTree &t){return (t.truth_barcode() > 0 && t.truth_barcode() < 200000);});
}

Selection<IDPVMTree> IDPVMSelections::forResolution() {  
   return (IDPVMSelections::hasTrack() && IDPVMSelections::hasTruth() && 
           IDPVMSelections::passedTrack() && IDPVMSelections::passedTruth() && IDPVMSelections::isMatched() && IDPVMSelections::isAssociated() &&
           IDPVMSelections::isPrimary());
}

Selection<IDPVMTree> IDPVMSelections::forFakeRateDen() {  
   return (IDPVMSelections::hasTrack() && IDPVMSelections::passedTrack());
}

Selection<IDPVMTree> IDPVMSelections::forFakeRateNum() {  
   return (IDPVMSelections::forFakeRateDen() && IDPVMSelections::isFake());
}

Selection<IDPVMTree> IDPVMSelections::forEfficiencyDen() {  
   return (IDPVMSelections::hasTruth() && IDPVMSelections::passedTruth());
}

Selection<IDPVMTree> IDPVMSelections::forEfficiencyNum() {
   return (IDPVMSelections::forEfficiencyDen() && IDPVMSelections::passedTrack() && IDPVMSelections::isMatched() && IDPVMSelections::isAssociated());
}