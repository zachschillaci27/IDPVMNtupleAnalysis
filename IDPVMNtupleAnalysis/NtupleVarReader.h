#pragma once 
#include "IDPVMNtupleAnalysis/IDPVMTree.h"
#include "IDPVMNtupleAnalysis/IDPVMTemplates.h"
#include <functional> 

/// This method will generate a function to read the correct branch of the Ntuple corresponding to the passed var and level.
/// In case of an unsupported input, will print an error and return a dummy function returning 0. 

namespace NtupleVarReaderProvider{
    std::function<double(IDPVMTree &t)> generateVarReader(IDPVMDefs::variable var, IDPVMDefs::level level);
}
