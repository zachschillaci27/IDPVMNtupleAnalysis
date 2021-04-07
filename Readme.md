# Overview

IDPVMNtupleAnalysis is a (hopefully) user-friendly package to read flat Ntuples produced within r22 IDPVM via the command-line flag '--doTruthToRecoNtuple' ([see here](https://gitlab.cern.ch/atlas/athena/-/blob/master/InnerDetector/InDetValidation/InDetPhysValMonitoring/share/InDetPhysValMonitoring_topOptions.py#L13)). The package has functionality to reproduce various IDPVM tracking performance plots (e.g. track-/truth-level distributions, efficiencies, linked/unlinked fake rates, resolutions, and pull means/widths). Standard implementations of binnings, histograms, and selections are defined in headers for ease of use. The package relies on the submodule [NtupleAnalysisUtils](https://gitlab.cern.ch/goblirsc/NtupleAnalysisUtils) to handle the managing of histograms with some additional plotting support. Please refer to the example [here](https://gitlab.cern.ch/goblirsc/NtupleAnalysisUtils/blob/master/util/NtupleAnalysisUtils_Example.cxx) to get an idea of the functionality provided by NtupleAnalysisUtils.

# How to set up

The package should be built within CMake via a suitable analysis release (e.g. AthAnalysis, 21.2.156). The package can be cloned from GitLab via

```shell
git clone https://:@gitlab.cern.ch:8443/zschilla/IDPVMNtupleAnalysis.git
```

Within the cloned repository, fetch the submodule via

```shell
git submodule update --init --recursive
```

# Headers & Classes

## IDPVMTree

This class outlines the structure of the IDPVM Ntuple and allows for fast accessing of the various branches throughout the code.

## IDPVMTemplates

Definition of `IDPVMDefs` enums for easier variable manipulation. Location of standard IDPVM templates (as copied from r22 master) - including 1D binnings, 1D and 2D histograms, variable names, and axis labels.

## NtupleVarReader

This handles the linking of `IDPVMDefs` variables to their actual branches in the tree.

## IDPVMUtilities

Various helper methods for loading the classic histograms from the IDPVM output file. Useful for doing validations against standard IDPVM plots. Also somer helper methods for modifications of these histograms.

## IDPVMPlotUtils

Basic plotting methods for comparison of ntuple-based and classic IDPVM plots.

## IDPVMPopulators

Plot filler definitions (as defined in NtupleAnalysisUtils) used for populating resolution and pull histograms.

## IDPVMSelections

Common selection definitions (as defined in NtupleAnalysisUtils) for replicating various IDPVM plots.

## ResolutionHelper

Helper methods to compute the iterative RMS for resolutions and pulls.

# Executables

See the `util/ValidateIDPVMTree/` folder for examples of plotting various IDPVM plots using the ntuple-based approach. Used for validating against IDPVM results.

There are additional plotting macros used to produce some ATLAS public results within `util/Public`.
* `util/Public/ITkPerformancePlots.cxx`: ITk performance plots for February 2020 LHCC conference
* `util/Public/ITkPubNotePlots.cxx`: ITk performance plots for Spring 2021 PubNote
* `util/Public/IDSoftwarePubPlots.cxx`: ID software disk-usage plots for Spring 2021 vCHEP conference