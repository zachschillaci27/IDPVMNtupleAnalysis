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

Selection<IDPVMTree> IDPVMSelections::isPrimary() {  
   return Selection<IDPVMTree>("isPrimary",[](IDPVMTree &t){return (t.truth_barcode() > 0 && t.truth_barcode() < 200000);});
}

Selection<IDPVMTree> IDPVMSelections::isSecondary() {  
   return Selection<IDPVMTree>("isSecondary",[](IDPVMTree &t){return (t.truth_barcode() >= 200000);});
}

Selection<IDPVMTree> IDPVMSelections::forResolution() {  
   return (IDPVMSelections::isAssociated() && IDPVMSelections::passedTrack() && IDPVMSelections::passedTruth() && 
           IDPVMSelections::isMatched() && IDPVMSelections::isPrimary());
}

Selection<IDPVMTree> IDPVMSelections::forFakeRateDen() {  
   return (IDPVMSelections::isAssociated() && IDPVMSelections::passedTrack());
}

Selection<IDPVMTree> IDPVMSelections::forFakeRateNum() {  
   return (IDPVMSelections::forFakeRateDen() && IDPVMSelections::isFake());
}

Selection<IDPVMTree> IDPVMSelections::forEfficiencyDen() {  
   return (IDPVMSelections::hasTruth() && IDPVMSelections::passedTruth());
}

Selection<IDPVMTree> IDPVMSelections::forEfficiencyNum() {
   return (IDPVMSelections::forEfficiencyDen() && IDPVMSelections::hasTrack() && IDPVMSelections::passedTrack() && IDPVMSelections::isMatched());
}