#include "IDPVMNtupleAnalysis/IDPVMUtilities.h"

const std::string GetPlotTitle(const std::string & plotname) {
    return plotname.substr(plotname.find_last_of("/") + 1);
}

Plot<TH1> LoadIDPVMEfficiency(const std::string & myphysval, const std::string & plotname) {

    Plot<TEfficiency> h_temp = LoadIDPVMHistogram<TEfficiency>(myphysval, plotname);

    // convert to TH1 for easier plotting
    TH1* h_out = PlotUtils::getRatio(h_temp->GetPassedHistogram()->Clone("passed"), h_temp->GetTotalHistogram()->Clone("total"), PlotUtils::efficiencyErrors);

    Plot<TH1> thePlot(GetPlotTitle(plotname), h_out);
    delete h_out;

    return thePlot;
}