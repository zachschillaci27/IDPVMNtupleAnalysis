#include "IDPVMNtupleAnalysis/IDPVMSelections.h"

Selection<IDPVMTree> IDPVMSelections::dummy() {  
   return Selection<IDPVMTree>("dummy",[](IDPVMTree &){return true;});
}

Selection<IDPVMTree> IDPVMSelections::hasTrack() {  
   return Selection<IDPVMTree>("hasTrack",[](IDPVMTree &t){return t.hasTrack();});
}

Selection<IDPVMTree> IDPVMSelections::hasTruth() {  
   return Selection<IDPVMTree>("hasTruth",[](IDPVMTree &t){return t.hasTruth();});
}

Selection<IDPVMTree> IDPVMSelections::isAssociated() {  
   return (IDPVMSelections::hasTrack() && IDPVMSelections::hasTruth());
}

Selection<IDPVMTree> IDPVMSelections::isBestMatched() {  
   return Selection<IDPVMTree>("isBestMatched",[](IDPVMTree &t){return t.track_truthMatchRanking() == 0;});
}

Selection<IDPVMTree> IDPVMSelections::isTruthDuplicate() {  
   return (IDPVMSelections::isAssociated() && !IDPVMSelections::isBestMatched());
}

Selection<IDPVMTree> IDPVMSelections::passedTrack() {  
   return (IDPVMSelections::hasTrack() && Selection<IDPVMTree>("passedTrack",[](IDPVMTree &t){return t.passedTrackSelection();}));
}

Selection<IDPVMTree> IDPVMSelections::passedTruth() {  
   return (IDPVMSelections::hasTruth() && Selection<IDPVMTree>("passedTruth",[](IDPVMTree &t){return t.passedTruthSelection();}));
}

Selection<IDPVMTree> IDPVMSelections::isSelectedByPileupSwitch() {  
   return Selection<IDPVMTree>("isSelectedByPileupSwitch",[](IDPVMTree &t){return t.truth_selectedByPileupSwitch();});
}

Selection<IDPVMTree> IDPVMSelections::isMatched() {  
   return Selection<IDPVMTree>("isMatched",[](IDPVMTree &t){return (t.track_truthMatchProb() > 0.5);});
}

Selection<IDPVMTree> IDPVMSelections::isFake() {  
   return Selection<IDPVMTree>("isFake",[](IDPVMTree &t){return (t.track_truthMatchProb() < 0.5);});
}

Selection<IDPVMTree> IDPVMSelections::isPrimary() {  
   return Selection<IDPVMTree>("isPrimary",[](IDPVMTree &t){return (t.truth_barcode() > 0 && t.truth_barcode() < 200000);});
}

Selection<IDPVMTree> IDPVMSelections::isSecondary() {  
   return Selection<IDPVMTree>("isSecondary",[](IDPVMTree &t){return (t.truth_barcode() >= 200000);});
}

Selection<IDPVMTree> IDPVMSelections::forResolution() {  
   return (IDPVMSelections::passedTruth() && IDPVMSelections::passedTrack() && IDPVMSelections::isMatched() && IDPVMSelections::isPrimary());
}

Selection<IDPVMTree> IDPVMSelections::forUnlinkedRateDen() {
   return IDPVMSelections::passedTrack();
}

Selection<IDPVMTree> IDPVMSelections::forUnlinkedRateNum() {
   return (IDPVMSelections::forUnlinkedRateDen() && !IDPVMSelections::hasTruth());
}

Selection<IDPVMTree> IDPVMSelections::forFakeRateDen() {  
   return (IDPVMSelections::passedTrack() && IDPVMSelections::isAssociated());
}

Selection<IDPVMTree> IDPVMSelections::forFakeRateNum() {  
   return (IDPVMSelections::forFakeRateDen() && IDPVMSelections::isFake());
}

Selection<IDPVMTree> IDPVMSelections::forEfficiencyDen() {  
   return (IDPVMSelections::passedTruth() && IDPVMSelections::isSelectedByPileupSwitch() && !IDPVMSelections::isTruthDuplicate());
}

Selection<IDPVMTree> IDPVMSelections::forEfficiencyNum() {
   return (IDPVMSelections::forEfficiencyDen() && IDPVMSelections::isMatched());
}