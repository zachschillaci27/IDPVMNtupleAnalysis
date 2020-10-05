#include "IDPVMNtupleAnalysis/NtupleVarReader.h"

namespace NtupleVarReaderProvider{
    std::function<double(IDPVMTree &t)> generateVarReader(IDPVMDefs::variable var, IDPVMDefs::level level) {

        static std::map<std::pair<IDPVMDefs::variable, IDPVMDefs::level>, std::function<double(IDPVMTree&)>> readerMap{
             // Track-level
             {std::make_pair(IDPVMDefs::eta,     IDPVMDefs::track), [](IDPVMTree &t){ return t.track_eta(); }},
             {std::make_pair(IDPVMDefs::pt,      IDPVMDefs::track), [](IDPVMTree &t){ return t.track_pt() / 1000.; }},
             {std::make_pair(IDPVMDefs::d0,      IDPVMDefs::track), [](IDPVMTree &t){ return t.track_d0(); }},
             {std::make_pair(IDPVMDefs::z0,      IDPVMDefs::track), [](IDPVMTree &t){ return t.track_z0(); }},
             {std::make_pair(IDPVMDefs::z0sin,   IDPVMDefs::track), [](IDPVMTree &t){ return t.track_z0sin(); }},
             {std::make_pair(IDPVMDefs::phi,     IDPVMDefs::track), [](IDPVMTree &t){ return t.track_phi(); }},
             {std::make_pair(IDPVMDefs::z0sin,   IDPVMDefs::track), [](IDPVMTree &t){ return t.track_theta(); }},
             {std::make_pair(IDPVMDefs::qOverPt, IDPVMDefs::track), [](IDPVMTree &t){ return t.track_qOverPt(); }},
             {std::make_pair(IDPVMDefs::qOverP,  IDPVMDefs::track), [](IDPVMTree &t){ return t.track_qOverP(); }},
             // Truth-level
             {std::make_pair(IDPVMDefs::eta,     IDPVMDefs::truth), [](IDPVMTree &t){ return t.truth_eta(); }},
             {std::make_pair(IDPVMDefs::pt,      IDPVMDefs::truth), [](IDPVMTree &t){ return t.truth_pt() / 1000.; }},
             {std::make_pair(IDPVMDefs::d0,      IDPVMDefs::truth), [](IDPVMTree &t){ return t.truth_d0(); }},
             {std::make_pair(IDPVMDefs::z0,      IDPVMDefs::truth), [](IDPVMTree &t){ return t.truth_z0(); }},
             {std::make_pair(IDPVMDefs::z0sin,   IDPVMDefs::truth), [](IDPVMTree &t){ return t.truth_z0sin(); }},
             {std::make_pair(IDPVMDefs::phi,     IDPVMDefs::truth), [](IDPVMTree &t){ return t.truth_phi(); }},
             {std::make_pair(IDPVMDefs::z0sin,   IDPVMDefs::truth), [](IDPVMTree &t){ return t.truth_theta(); }},
             {std::make_pair(IDPVMDefs::qOverPt, IDPVMDefs::truth), [](IDPVMTree &t){ return t.truth_qOverPt(); }},
             {std::make_pair(IDPVMDefs::qOverP,  IDPVMDefs::truth), [](IDPVMTree &t){ return t.truth_qOverP(); }},
        };

        auto found = readerMap.find(std::make_pair(var, level));
        if (found == readerMap.end()){
            std::cerr << "Error: generateVarReader does not yet support the combination of variable " << IDPVMLabels::getVarName(var) << " and level " << IDPVMLabels::getVarName(level) << ". -  Will return dummy function instead! " << std::endl;
            return [](IDPVMTree &){ return 0; };
        }
        return found->second;
    }
}
    