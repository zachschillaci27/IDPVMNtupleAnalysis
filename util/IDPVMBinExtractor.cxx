#include "IDPVMNtupleAnalysis/IDPVMUtilities.h"
#include "TH1F.h"

template <class HistoType> std::vector<double> ExtractBinArray(HistoType histo) {
    
    const unsigned int nbins = histo->GetXaxis()->GetNbins();
    
    std::vector<double> bins = {};
    for (unsigned int i = 1; i < nbins; ++i) { bins.push_back(histo->GetXaxis()->GetBinLowEdge(i)); }
    bins.push_back(histo->GetXaxis()->GetBinUpEdge(nbins));
    
    return bins;
}

std::string FormatBinArray(const std::vector<double> & bins) {
    
    std::string fmtbins = "{";
    for (auto & b : bins) { fmtbins += std::to_string(b) + ","; }
    fmtbins = fmtbins.substr(0, fmtbins.size() - 1) + "} \n";
    
    fmtbins += "nbins: "           + std::to_string(bins.size() - 1)   + "\n" +
               "first bin width: " + std::to_string(bins[1] - bins[0]) + "\n" +
               "last bin width: "  + std::to_string(bins[bins.size() - 1] - bins[bins.size() - 2]);

    return fmtbins;
}

template <class HistoType> void DumpBinArray(HistoType histo) {

    std::vector<double> bins = ExtractBinArray<HistoType>(histo);
    std::string fmtbins = FormatBinArray(bins);
    
    std::cout << fmtbins << std::endl;

    return;
}

int main (int, char**){

    const std::string myphysval = "/Users/zschillaci/CERN/Working/Datasets/Tracking/IDPVM/Results/MyPhysVal.root";

    bool th1f = true;

    //TH1F*
    if (th1f) {
        const std::vector<std::string> plotnames = {
            "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0resolutionRMS_vs_eta",
            "IDPerformanceMon/Tracks/SelectedMatchedTracks/Primary/z0pullmeanRMS_vs_eta",
            "IDPerformanceMon/Tracks/SelectedGoodTracks/truthphi",
            "IDPerformanceMon/Tracks/SelectedGoodTracks/truththeta",
        };

        const std::map<std::string, Plot<TH1>> histograms = LoadIDPVMHistograms<TH1>(myphysval, plotnames);

        for (auto & histo : histograms) {
            std::cout << "Getting binning for " << histo.first << " ... " << std::endl;
            DumpBinArray<Plot<TH1>>(histo.second);
        }
    }

    return 0;
}
