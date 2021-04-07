#include "NtupleAnalysisUtils/NtupleBranch.h"
#include "NtupleAnalysisUtils/NtupleBranchAccessByName.h"
#include "NtupleAnalysisUtils/Selection.h"
#include "NtupleAnalysisUtils/PlotFillInstruction.h"
#include "NtupleAnalysisUtils/PlotContent.h"
#include "NtupleAnalysisUtils/Sample.h"
#include "NtupleAnalysisUtils/PlotUtils.h"
#include "NtupleAnalysisUtils/CanvasOptions.h"
#include "NtupleAnalysisUtils/AtlasStyle.h"
#include "NtupleAnalysisUtils/DefaultPlotting.h"
#include "IDPVMNtupleAnalysis/IDPVMUtilities.h"

class SquirrelTree : public NtupleBranchMgr {
    public:
    SquirrelTree( TTree* t): NtupleBranchMgr(t),
        Year("Year", t, this),
        Run("Run", t, this),
        Mu("Mu", t, this),
        TotalDisk("TotalDisk", t, this),
        DiskFromLRT("DiskFromLRT", t, this),
        inSeeds("inSeeds", t, this),
        outTracks("outTracks", t, this),
        CPU_InDetTRT_ExtensionDisappearing("CPU_InDetTRT_ExtensionDisappearing", t, this),
        CPU_InDetSCTEventFlagWriter("CPU_InDetSCTEventFlagWriter", t, this),
        CPU_InDetExtensionProcessorDisappearing("CPU_InDetExtensionProcessorDisappearing", t, this),
        CPU_InDetBCM_ZeroSuppression("CPU_InDetBCM_ZeroSuppression", t, this),
        CPU_InDetAmbiguityScoreForwardTracks("CPU_InDetAmbiguityScoreForwardTracks", t, this),
        CPU_InDetROIInfoVecCondAlg("CPU_InDetROIInfoVecCondAlg", t, this),
        CPU_InDetDisappearingTrackParticles("CPU_InDetDisappearingTrackParticles", t, this),
        CPU_InDetBCMRawDataProvider("CPU_InDetBCMRawDataProvider", t, this),
        CPU_ThinInDetForwardTrackParticlesAlg("CPU_ThinInDetForwardTrackParticlesAlg", t, this),
        CPU_InDetTRT_SeededAmbiguityScore("CPU_InDetTRT_SeededAmbiguityScore", t, this),
        CPU_InDetCopyAlg("CPU_InDetCopyAlg", t, this),
        CPU_InDetSCT_DCSConditionsHVCondAlg("CPU_InDetSCT_DCSConditionsHVCondAlg", t, this),
        CPU_InDetSCT_DCSConditionsStatCondAlg("CPU_InDetSCT_DCSConditionsStatCondAlg", t, this),
        CPU_InDetSCT_DCSConditionsTempCondAlg("CPU_InDetSCT_DCSConditionsTempCondAlg", t, this),
        CPU_InDetSiDetElementBoundaryLinksPixelCondAlg("CPU_InDetSiDetElementBoundaryLinksPixelCondAlg", t, this),
        CPU_InDetSiDetElementBoundaryLinksSCTCondAlg("CPU_InDetSiDetElementBoundaryLinksSCTCondAlg", t, this),
        CPU_InDetSiElementPropertiesTableCondAlg("CPU_InDetSiElementPropertiesTableCondAlg", t, this),
        CPU_InDet__SiDetElementsRoadCondAlg_xk("CPU_InDet__SiDetElementsRoadCondAlg_xk", t, this),
        CPU_InDet__TRT_DetElementsRoadCondAlg_xk("CPU_InDet__TRT_DetElementsRoadCondAlg_xk", t, this),
        CPU_InDet__TRT_TrackSegmentsMakerCondAlg_ATLxk("CPU_InDet__TRT_TrackSegmentsMakerCondAlg_ATLxk", t, this),
        CPU_CaloThinCellsByClusterAlg_InDetTrackParticlesAssociatedClusters("CPU_CaloThinCellsByClusterAlg_InDetTrackParticlesAssociatedClusters", t, this),
        CPU_InDetAmbiguityScoreDisappearing("CPU_InDetAmbiguityScoreDisappearing", t, this),
        CPU_InDetAmbiguitySolverDisappearing("CPU_InDetAmbiguitySolverDisappearing", t, this),
        CPU_InDetUsedInFitDecorator("CPU_InDetUsedInFitDecorator", t, this),
        CPU_InDetTRT_SeededAmbiguitySolver("CPU_InDetTRT_SeededAmbiguitySolver", t, this),
        CPU_InDetTrackCollectionMerger_pix("CPU_InDetTrackCollectionMerger_pix", t, this),
        CPU_InDetForwardTrackParticles("CPU_InDetForwardTrackParticles", t, this),
        CPU_InDetCaloClusterROISelector("CPU_InDetCaloClusterROISelector", t, this),
        CPU_InDetAmbiguityScoreR3LargeD0("CPU_InDetAmbiguityScoreR3LargeD0", t, this),
        CPU_InDetLargeD0TrackParticles("CPU_InDetLargeD0TrackParticles", t, this),
        CPU_InDetTrackPRD_AssociationR3LargeD0("CPU_InDetTrackPRD_AssociationR3LargeD0", t, this),
        CPU_InDetTrackPRD_AssociationForwardTracks("CPU_InDetTrackPRD_AssociationForwardTracks", t, this),
        CPU_InDetTrackSlimmer("CPU_InDetTrackSlimmer", t, this),
        CPU_InDetTRT_StandaloneTrackFinder("CPU_InDetTRT_StandaloneTrackFinder", t, this),
        CPU_InDetTRT_SeededTrackFinder("CPU_InDetTRT_SeededTrackFinder", t, this),
        CPU_InDetSegmentTrackPRD_Association("CPU_InDetSegmentTrackPRD_Association", t, this),
        CPU_InDetTRTonly_TrackPRD_Association("CPU_InDetTRTonly_TrackPRD_Association", t, this),
        CPU_InDetTrackPRD_AssociationDisappearing("CPU_InDetTrackPRD_AssociationDisappearing", t, this),
        CPU_InDetTRT_TrackSegmentsFinder("CPU_InDetTRT_TrackSegmentsFinder", t, this),
        CPU_InDetSCTRawDataProvider("CPU_InDetSCTRawDataProvider", t, this),
        CPU_InDetTrackParticles("CPU_InDetTrackParticles", t, this),
        CPU_InDetLowBetaTrkAlgorithm("CPU_InDetLowBetaTrkAlgorithm", t, this),
        CPU_InDetTRT_ExtensionR3LargeD0("CPU_InDetTRT_ExtensionR3LargeD0", t, this),
        CPU_InDetAmbiguityScore("CPU_InDetAmbiguityScore", t, this),
        CPU_InDetSiSpTrackFinderDisappearing("CPU_InDetSiSpTrackFinderDisappearing", t, this),
        CPU_InDetRecStatistics("CPU_InDetRecStatistics", t, this),
        CPU_InDetSiSpTrackFinderForwardTracks("CPU_InDetSiSpTrackFinderForwardTracks", t, this),
        CPU_InDetAmbiguitySolverForwardTracks("CPU_InDetAmbiguitySolverForwardTracks", t, this),
        CPU_InDetPixelRawDataProvider("CPU_InDetPixelRawDataProvider", t, this),
        CPU_InDetSiTrackerSpacePointFinder("CPU_InDetSiTrackerSpacePointFinder", t, this),
        CPU_InDetTRTRawDataProvider("CPU_InDetTRTRawDataProvider", t, this),
        CPU_InDetPriVxFinder("CPU_InDetPriVxFinder", t, this),
        CPU_InDetSCT_Clusterization("CPU_InDetSCT_Clusterization", t, this),
        CPU_InDetPixelClusterization("CPU_InDetPixelClusterization", t, this),
        CPU_InDetTrackCollectionMerger("CPU_InDetTrackCollectionMerger", t, this),
        CPU_InDetExtensionProcessorR3LargeD0("CPU_InDetExtensionProcessorR3LargeD0", t, this),
        CPU_InDetAmbiguitySolverR3LargeD0("CPU_InDetAmbiguitySolverR3LargeD0", t, this),
        CPU_InDetTRT_Extension("CPU_InDetTRT_Extension", t, this),
        CPU_InDetTRT_RIO_Maker("CPU_InDetTRT_RIO_Maker", t, this),
        CPU_InDetExtensionProcessor("CPU_InDetExtensionProcessor", t, this),
        CPU_InDetSiSpTrackFinderR3LargeD0("CPU_InDetSiSpTrackFinderR3LargeD0", t, this),
        CPU_InDetSiSpTrackFinder("CPU_InDetSiSpTrackFinder", t, this),
        CPU_InDetAmbiguitySolver("CPU_InDetAmbiguitySolver", t, this),
        CPU_AllLRT("CPU_AllLRT", t, this),
        CPU_Total("CPU_Total", t, this) {
        if(t) getMissedBranches(t);
    }
    NtupleBranch<Int_t>           Year;
    NtupleBranch<Int_t>           Run;
    NtupleBranch<Float_t>         Mu;
    NtupleBranch<Float_t>         TotalDisk;
    NtupleBranch<Float_t>         DiskFromLRT;
    NtupleBranch<Int_t>           inSeeds;
    NtupleBranch<Int_t>           outTracks;
    NtupleBranch<Float_t>         CPU_InDetTRT_ExtensionDisappearing;
    NtupleBranch<Float_t>         CPU_InDetSCTEventFlagWriter;
    NtupleBranch<Float_t>         CPU_InDetExtensionProcessorDisappearing;
    NtupleBranch<Float_t>         CPU_InDetBCM_ZeroSuppression;
    NtupleBranch<Float_t>         CPU_InDetAmbiguityScoreForwardTracks;
    NtupleBranch<Float_t>         CPU_InDetROIInfoVecCondAlg;
    NtupleBranch<Float_t>         CPU_InDetDisappearingTrackParticles;
    NtupleBranch<Float_t>         CPU_InDetBCMRawDataProvider;
    NtupleBranch<Float_t>         CPU_ThinInDetForwardTrackParticlesAlg;
    NtupleBranch<Float_t>         CPU_InDetTRT_SeededAmbiguityScore;
    NtupleBranch<Float_t>         CPU_InDetCopyAlg;
    NtupleBranch<Float_t>         CPU_InDetSCT_DCSConditionsHVCondAlg;
    NtupleBranch<Float_t>         CPU_InDetSCT_DCSConditionsStatCondAlg;
    NtupleBranch<Float_t>         CPU_InDetSCT_DCSConditionsTempCondAlg;
    NtupleBranch<Float_t>         CPU_InDetSiDetElementBoundaryLinksPixelCondAlg;
    NtupleBranch<Float_t>         CPU_InDetSiDetElementBoundaryLinksSCTCondAlg;
    NtupleBranch<Float_t>         CPU_InDetSiElementPropertiesTableCondAlg;
    NtupleBranch<Float_t>         CPU_InDet__SiDetElementsRoadCondAlg_xk;
    NtupleBranch<Float_t>         CPU_InDet__TRT_DetElementsRoadCondAlg_xk;
    NtupleBranch<Float_t>         CPU_InDet__TRT_TrackSegmentsMakerCondAlg_ATLxk;
    NtupleBranch<Float_t>         CPU_CaloThinCellsByClusterAlg_InDetTrackParticlesAssociatedClusters;
    NtupleBranch<Float_t>         CPU_InDetAmbiguityScoreDisappearing;
    NtupleBranch<Float_t>         CPU_InDetAmbiguitySolverDisappearing;
    NtupleBranch<Float_t>         CPU_InDetUsedInFitDecorator;
    NtupleBranch<Float_t>         CPU_InDetTRT_SeededAmbiguitySolver;
    NtupleBranch<Float_t>         CPU_InDetTrackCollectionMerger_pix;
    NtupleBranch<Float_t>         CPU_InDetForwardTrackParticles;
    NtupleBranch<Float_t>         CPU_InDetCaloClusterROISelector;
    NtupleBranch<Float_t>         CPU_InDetAmbiguityScoreR3LargeD0;
    NtupleBranch<Float_t>         CPU_InDetLargeD0TrackParticles;
    NtupleBranch<Float_t>         CPU_InDetTrackPRD_AssociationR3LargeD0;
    NtupleBranch<Float_t>         CPU_InDetTrackPRD_AssociationForwardTracks;
    NtupleBranch<Float_t>         CPU_InDetTrackSlimmer;
    NtupleBranch<Float_t>         CPU_InDetTRT_StandaloneTrackFinder;
    NtupleBranch<Float_t>         CPU_InDetTRT_SeededTrackFinder;
    NtupleBranch<Float_t>         CPU_InDetSegmentTrackPRD_Association;
    NtupleBranch<Float_t>         CPU_InDetTRTonly_TrackPRD_Association;
    NtupleBranch<Float_t>         CPU_InDetTrackPRD_AssociationDisappearing;
    NtupleBranch<Float_t>         CPU_InDetTRT_TrackSegmentsFinder;
    NtupleBranch<Float_t>         CPU_InDetSCTRawDataProvider;
    NtupleBranch<Float_t>         CPU_InDetTrackParticles;
    NtupleBranch<Float_t>         CPU_InDetLowBetaTrkAlgorithm;
    NtupleBranch<Float_t>         CPU_InDetTRT_ExtensionR3LargeD0;
    NtupleBranch<Float_t>         CPU_InDetAmbiguityScore;
    NtupleBranch<Float_t>         CPU_InDetSiSpTrackFinderDisappearing;
    NtupleBranch<Float_t>         CPU_InDetRecStatistics;
    NtupleBranch<Float_t>         CPU_InDetSiSpTrackFinderForwardTracks;
    NtupleBranch<Float_t>         CPU_InDetAmbiguitySolverForwardTracks;
    NtupleBranch<Float_t>         CPU_InDetPixelRawDataProvider;
    NtupleBranch<Float_t>         CPU_InDetSiTrackerSpacePointFinder;
    NtupleBranch<Float_t>         CPU_InDetTRTRawDataProvider;
    NtupleBranch<Float_t>         CPU_InDetPriVxFinder;
    NtupleBranch<Float_t>         CPU_InDetSCT_Clusterization;
    NtupleBranch<Float_t>         CPU_InDetPixelClusterization;
    NtupleBranch<Float_t>         CPU_InDetTrackCollectionMerger;
    NtupleBranch<Float_t>         CPU_InDetExtensionProcessorR3LargeD0;
    NtupleBranch<Float_t>         CPU_InDetAmbiguitySolverR3LargeD0;
    NtupleBranch<Float_t>         CPU_InDetTRT_Extension;
    NtupleBranch<Float_t>         CPU_InDetTRT_RIO_Maker;
    NtupleBranch<Float_t>         CPU_InDetExtensionProcessor;
    NtupleBranch<Float_t>         CPU_InDetSiSpTrackFinderR3LargeD0;
    NtupleBranch<Float_t>         CPU_InDetSiSpTrackFinder;
    NtupleBranch<Float_t>         CPU_InDetAmbiguitySolver;
    NtupleBranch<Float_t>         CPU_AllLRT;
    NtupleBranch<Float_t>         CPU_Total;
};

Sample<SquirrelTree> getSample(const std::string & logfile) {
    const std::string tree  = "Squirrels";
    return Sample<SquirrelTree>(logfile, logfile, tree);
}

TH1F getMuHist() {
    static const std::vector<Float_t> mu{15.0,16.0,24.5,25.5,29.5,30.5,49.5,50.5,54.2,55.2,59.5,60.5,69.5,70.5,79.6,80.6,90.5};
    TH1F h_mu("","",mu.size()-1,&(mu[0]));
    return h_mu;
}

float getNevents(float mu) {
    if ((mu == 15.5) || (mu == 25.0) || (mu == 30.0) || (mu == 50.0) || (mu == 70.0)) return 300.;
    return 100.;
}

void compareCPU(const Sample<SquirrelTree> & baseSample, const Sample<SquirrelTree> & newSample, const std::string stream) {
    const std::vector<std::string> AllCPU{
        // "CPU_InDetTRT_ExtensionDisappearing",
        // "CPU_InDetSCTEventFlagWriter",
        // "CPU_InDetExtensionProcessorDisappearing",
        // "CPU_InDetBCM_ZeroSuppression",
        // "CPU_InDetAmbiguityScoreForwardTracks",
        // "CPU_InDetROIInfoVecCondAlg",
        // "CPU_InDetDisappearingTrackParticles",
        // "CPU_InDetBCMRawDataProvider",
        // "CPU_ThinInDetForwardTrackParticlesAlg",
        // "CPU_InDetTRT_SeededAmbiguityScore",
        // "CPU_InDetCopyAlg",
        // "CPU_InDetSCT_DCSConditionsHVCondAlg",
        // "CPU_InDetSCT_DCSConditionsStatCondAlg",
        // "CPU_InDetSCT_DCSConditionsTempCondAlg",
        // "CPU_InDetSiDetElementBoundaryLinksPixelCondAlg",
        // "CPU_InDetSiDetElementBoundaryLinksSCTCondAlg",
        // "CPU_InDetSiElementPropertiesTableCondAlg",
        // "CPU_InDet__SiDetElementsRoadCondAlg_xk",
        // "CPU_InDet__TRT_DetElementsRoadCondAlg_xk",
        // "CPU_InDet__TRT_TrackSegmentsMakerCondAlg_ATLxk",
        // "CPU_CaloThinCellsByClusterAlg_InDetTrackParticlesAssociatedClusters",
        // "CPU_InDetAmbiguityScoreDisappearing",
        /*
        "CPU_InDetAmbiguitySolverDisappearing",
        "CPU_InDetUsedInFitDecorator",
        "CPU_InDetTRT_SeededAmbiguitySolver",
        "CPU_InDetTrackCollectionMerger_pix",
        "CPU_InDetForwardTrackParticles",
        "CPU_InDetCaloClusterROISelector",
        "CPU_InDetAmbiguityScoreR3LargeD0",
        "CPU_InDetLargeD0TrackParticles",
        "CPU_InDetTrackPRD_AssociationR3LargeD0",
        "CPU_InDetTrackPRD_AssociationForwardTracks",
        "CPU_InDetTrackSlimmer",
        "CPU_InDetTRT_StandaloneTrackFinder",
        "CPU_InDetTRT_SeededTrackFinder",
        "CPU_InDetSegmentTrackPRD_Association",
        "CPU_InDetTRTonly_TrackPRD_Association",
        "CPU_InDetTrackPRD_AssociationDisappearing",
        "CPU_InDetTRT_TrackSegmentsFinder",
        "CPU_InDetSCTRawDataProvider",
        "CPU_InDetTrackParticles",
        "CPU_InDetLowBetaTrkAlgorithm",
        "CPU_InDetTRT_ExtensionR3LargeD0",
        "CPU_InDetAmbiguityScore",
        "CPU_InDetSiSpTrackFinderDisappearing",
        "CPU_InDetRecStatistics",
        "CPU_InDetSiSpTrackFinderForwardTracks",
        "CPU_InDetAmbiguitySolverForwardTracks",
        "CPU_InDetPixelRawDataProvider",
        "CPU_InDetSiTrackerSpacePointFinder",
        "CPU_InDetTRTRawDataProvider",
        "CPU_InDetPriVxFinder",
        "CPU_InDetSCT_Clusterization",
        "CPU_InDetPixelClusterization",
        "CPU_InDetTrackCollectionMerger",
        "CPU_InDetExtensionProcessorR3LargeD0",
        "CPU_InDetAmbiguitySolverR3LargeD0",
        "CPU_InDetTRT_Extension",
        "CPU_InDetTRT_RIO_Maker",
        "CPU_InDetExtensionProcessor",
        "CPU_InDetSiSpTrackFinderR3LargeD0",
        "CPU_InDetSiSpTrackFinder",
        "CPU_InDetAmbiguitySolver",
        */
        "CPU_AllLRT",
        "CPU_Total",
    };

    auto h_mu = getMuHist();

    for (auto & cpu : AllCPU) {        
        NtupleBranchAccessByName<Float_t> accessor(cpu);
        PlotFillInstructionWithRef<TH1F, SquirrelTree> filler(cpu, [&accessor](TH1F* h, SquirrelTree& t){ 
            Float_t c = -1.; 
            accessor.getVal(t, c);
            h->Fill(t.Mu(), c); }, h_mu);
        
        PlotContent<TH1F> pc({
            Plot<TH1F>(baseSample, Selection<SquirrelTree>(), filler, "Baseline",        "PL", PremadePlotFormats::Markers1().ExtraDrawOpts(("HISTP"))),
            Plot<TH1F>(newSample,  Selection<SquirrelTree>(), filler, "Looser BLS Cuts", "PL", PremadePlotFormats::Markers2().ExtraDrawOpts(("HISTP"))),
        }, {"Athena 22.0.30", cpu, stream}, "newCutsForBLS/" + cpu + "-" + stream, "newCutsForBLS/newCutsForBLS", CanvasOptions().xAxisTitle("#LT#mu#GT").yAxisTitle("CPU Time").ratioAxisTitle("#frac{Loose}{Baseline}").extraXtitleOffset(.75).extraYtitleOffset(.1).yMin(1.e-3).ratioMin(1.e-3).yMaxExtraPadding(0.75));
        DefaultPlotting::draw1D(pc);
    }
}

void compareDisk(const Sample<SquirrelTree> & baseSample, const Sample<SquirrelTree> & newSample, const std::string stream) {
    
    auto h_mu = getMuHist();
    
    PlotFillInstructionWithRef<TH1F, SquirrelTree> diskTotFiller("diskTotFiller", [](TH1F* h, SquirrelTree& t){ h->Fill(t.Mu(), t.TotalDisk());   }, h_mu);
    PlotFillInstructionWithRef<TH1F, SquirrelTree> diskLRTFiller("diskLRTFiller", [](TH1F* h, SquirrelTree& t){ h->Fill(t.Mu(), t.DiskFromLRT()); }, h_mu);
        
    PlotContent<TH1F> pcTot({
        Plot<TH1F>(baseSample, Selection<SquirrelTree>(), diskTotFiller, "Baseline",        "PL", PremadePlotFormats::Markers1().ExtraDrawOpts(("HISTP"))),
        Plot<TH1F>(newSample,  Selection<SquirrelTree>(), diskTotFiller, "Looser BLS Cuts", "PL", PremadePlotFormats::Markers2().ExtraDrawOpts(("HISTP"))),
    }, {"Athena 22.0.30", "Total Disk", stream}, "newCutsForBLS/diskTot-" + stream, "newCutsForBLS/newCutsForBLS", CanvasOptions().xAxisTitle("#LT#mu#GT").yAxisTitle("Disk Space [kb/event]").ratioAxisTitle("#frac{Loose}{Baseline}").extraXtitleOffset(.75).extraYtitleOffset(.1).yMin(1.e-3).ratioMin(1.e-3).yMaxExtraPadding(0.75));
    DefaultPlotting::draw1D(pcTot);

    PlotContent<TH1F> pcLRT({
        Plot<TH1F>(baseSample, Selection<SquirrelTree>(), diskLRTFiller, "Baseline",        "PL", PremadePlotFormats::Markers1().ExtraDrawOpts(("HISTP"))),
        Plot<TH1F>(newSample,  Selection<SquirrelTree>(), diskLRTFiller, "Looser BLS Cuts", "PL", PremadePlotFormats::Markers2().ExtraDrawOpts(("HISTP"))),
    }, {"Athena 22.0.30", "LRT Disk", stream}, "newCutsForBLS/diskLRT-" + stream, "newCutsForBLS/newCutsForBLS", CanvasOptions().xAxisTitle("#LT#mu#GT").yAxisTitle("Disk Space [kb/event]").ratioAxisTitle("#frac{Loose}{Baseline}").extraXtitleOffset(.75).extraYtitleOffset(.1).yMin(1.e-3).ratioMin(1.e-3).yMaxExtraPadding(0.75));
    DefaultPlotting::draw1D(pcLRT);
}

void compareTracks(const Sample<SquirrelTree> & baseSample, const Sample<SquirrelTree> & newSample, const std::string stream) {
    
    auto h_mu = getMuHist();
    
    PlotFillInstructionWithRef<TH1F, SquirrelTree> seedsFiller("seedsFiller", [](TH1F* h, SquirrelTree& t){ h->Fill(t.Mu(), t.inSeeds()   / getNevents(t.Mu())); }, h_mu);
    PlotFillInstructionWithRef<TH1F, SquirrelTree> trackFiller("trackFiller", [](TH1F* h, SquirrelTree& t){ h->Fill(t.Mu(), t.outTracks() / getNevents(t.Mu())); }, h_mu);
        
    PlotContent<TH1F> pcTot({
        Plot<TH1F>(baseSample, Selection<SquirrelTree>(), seedsFiller, "Baseline",        "PL", PremadePlotFormats::Markers1().ExtraDrawOpts(("HISTP"))),
        Plot<TH1F>(newSample,  Selection<SquirrelTree>(), seedsFiller, "Looser BLS Cuts", "PL", PremadePlotFormats::Markers2().ExtraDrawOpts(("HISTP"))),
    }, {"Athena 22.0.30", "In Seeds", stream}, "newCutsForBLS/seeds-" + stream, "newCutsForBLS/newCutsForBLS", CanvasOptions().xAxisTitle("#LT#mu#GT").yAxisTitle("Seeds / Event").ratioAxisTitle("#frac{Loose}{Baseline}").extraXtitleOffset(.75).extraYtitleOffset(.1).yMin(1.e-3).ratioMin(1.e-3).yMaxExtraPadding(0.75));
    DefaultPlotting::draw1D(pcTot);

    PlotContent<TH1F> pcLRT({
        Plot<TH1F>(baseSample, Selection<SquirrelTree>(), trackFiller, "Baseline",        "PL", PremadePlotFormats::Markers1().ExtraDrawOpts(("HISTP"))),
        Plot<TH1F>(newSample,  Selection<SquirrelTree>(), trackFiller, "Looser BLS Cuts", "PL", PremadePlotFormats::Markers2().ExtraDrawOpts(("HISTP"))),
    }, {"Athena 22.0.30", "Out Tracks", stream}, "newCutsForBLS/tracks-" + stream, "newCutsForBLS/newCutsForBLS", CanvasOptions().xAxisTitle("#LT#mu#GT").yAxisTitle("Tracks / Event").ratioAxisTitle("#frac{Loose}{Baseline}").extraXtitleOffset(.75).extraYtitleOffset(.1).yMin(1.e-3).ratioMin(1.e-3).yMaxExtraPadding(0.75));
    DefaultPlotting::draw1D(pcLRT);
}

int main (int, char**) {

    SetAtlasStyle();

    // Baseline (Athena master - circa March 19, 2021)
    const std::string baseline      = "/home/zschilla/Working/Athena/Tracking/run/NewCutsForBLS/Baseline/Logs/SPOTResults.root";
    const std::string baselineBLS   = "/home/zschilla/Working/Athena/Tracking/run/NewCutsForBLS/Baseline/Logs/physics_BLS/SPOTResults.mu54.7_BLS.root";
    // Removing cut-levels 17/18 from baseline
    const std::string newCuts      = "/home/zschilla/Working/Athena/Tracking/run/NewCutsForBLS/NewCuts/Logs/SPOTResults.root";
    const std::string newCutsBLS   = "/home/zschilla/Working/Athena/Tracking/run/NewCutsForBLS/NewCuts/Logs/physics_BLS/SPOTResults.mu54.7_BLS.root";

    auto baseSample    = getSample(baseline);
    auto baseSampleBLS = getSample(baselineBLS);
    auto newSample     = getSample(newCuts);
    auto newSampleBLS  = getSample(newCutsBLS);

    PlotUtils::startMultiPagePdfFile("newCutsForBLS/newCutsForBLS");
    compareCPU(baseSample, newSample, "physics_main");
    compareDisk(baseSample, newSample, "physics_main");
    compareTracks(baseSample, newSample, "physics_main");
    compareCPU(baseSampleBLS, newSampleBLS, "physics_BLS");
    compareDisk(baseSampleBLS, newSampleBLS, "physics_BLS");
    compareTracks(baseSampleBLS, newSampleBLS, "physics_BLS");
    PlotUtils::endMultiPagePdfFile("newCutsForBLS/newCutsForBLS");

    return EXIT_SUCCESS;
}
