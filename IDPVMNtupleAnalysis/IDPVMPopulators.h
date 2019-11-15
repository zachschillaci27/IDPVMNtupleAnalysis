#ifndef __IDPVMPOPULATORS__H
#define __IDPVMPOPULATORS__H

#include "NtupleAnalysisUtils/PlotFillInstruction.h"
#include "IDPVMNtupleAnalysis/IDPVMTree.h"
#include "IDPVMNtupleAnalysis/IDPVMTemplates.h"

namespace IDPVMPopulators {
    PlotFillInstructionWithRef<TH2D, IDPVMTree> getResolutionPopulator(IDPVMDefs::variable var, IDPVMDefs::variable versus);
    PlotFillInstructionWithRef<TH2D, IDPVMTree> getPullPopulator(IDPVMDefs::variable var, IDPVMDefs::variable versus);
}

#endif


