#include "NtupleAnalysisUtils/Selection.h"
#include "NtupleAnalysisUtils/PlotFillInstruction.h"
#include "NtupleAnalysisUtils/PlotContent.h"
#include "NtupleAnalysisUtils/Sample.h"
#include "NtupleAnalysisUtils/PlotUtils.h"
#include "NtupleAnalysisUtils/CanvasOptions.h"
#include "NtupleAnalysisUtils/AtlasStyle.h"
#include "NtupleAnalysisUtils/DefaultPlotting.h"
#include "IDPVMNtupleAnalysis/IDPVMTree.h"
#include "IDPVMNtupleAnalysis/IDPVMTemplates.h"
#include "IDPVMNtupleAnalysis/IDPVMUtilities.h"
#include "IDPVMNtupleAnalysis/IDPVMSelections.h"
#include "TStopwatch.h"

int main (int, char**) {

    SetAtlasStyle();

    TStopwatch sw;

    const std::string myphysval = "/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/ttbar/alternative/MyPhysVal.root";
    Sample<IDPVMTree> ntuple("", myphysval, "IDPerformanceMon/Ntuples/IDPerformanceMon_NtuplesTruthToReco");   

    TH1D h_dummy("h_dummy", "", 1, 0.0, 1.0);
    PlotFillInstructionWithRef<TH1D, IDPVMTree> dummyFill("dummyFill", [](TH1D* h, IDPVMTree &){ h->Fill(0.5); }, h_dummy);

    std::cout << "Creating dummy plot ... " << std::endl;    
    Plot<TH1D> dummyPlot(ntuple, IDPVMSelections::dummy(), dummyFill);

    std::cout << "Populating ... " << std::endl;
    sw.Start();
    dummyPlot.populate();
    sw.Stop();

    double entries = dummyPlot->GetEntries();
    double time = sw.RealTime();
    double rate = entries / time / 1000.;

    std::cout << "Done!" << std::endl;
    std::cout << "Entries: " << std::to_string(entries) << std::endl;
    std::cout << "Time [s]: " << std::to_string(time) << std::endl;
    std::cout << "Rate [kHz]: " << std::to_string(rate) << std::endl;

    return 0;
}
