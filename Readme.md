# Overview

IDPVMNtupleAnalysis is a work-in-progress package to read flat Ntuples produced within an experimental branch of IDPVM available [here](https://gitlab.cern.ch/zschilla//athena/tree/NtupleForIDPVM). The package has functionality to reproduce various IDPVM tracking performance plots (e.g. track efficiencies, fake rates, resolutions, and pull means/widths). Standard implementations of binnings, histograms, and selections are defined in headers for ease of use. The package relies on the submodule [NtupleAnalysisUtils](https://gitlab.cern.ch/goblirsc/NtupleAnalysisUtils) to handle the filling of histograms. Please refer to the example [here](https://gitlab.cern.ch/goblirsc/NtupleAnalysisUtils/blob/master/util/NtupleAnalysisUtils_Example.cxx) to get an idea of the functionality provided by NtupleAnalysisUtils.

# How to set up

The package should be built within CMake via a suitable analysis release (e.g. AthAnalysis, 21.2.61). The package can be cloned from GitLab via

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

## IDPVMUtilities

Various helper methods for loading histograms from MyPhysVal.root. Useful for doing validations against standard IDPVM plots.

## IDPVMTemplates

Location of standard IDPVM templates - including 1D binnings, 1D and 2D histograms, variable names, and axis labels.

## IDPVMPopulators

Plot filler definitions (as defined in NtupleAnalysisUtils) used for populating resolution and pull histograms.

## IDPVMSelections

Common selection definitions (as defined in NtupleAnalysisUtils) for various IDPVM plots.

## ResolutionHelper

Helper methods to compute the iterative RMS for resolution and pull plots.