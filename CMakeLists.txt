################################################################################
# Package: IDPVMNtupleAnalysis
################################################################################

# Declare the package name:
atlas_subdir( IDPVMNtupleAnalysis )

atlas_depends_on_subdirs(
  PRIVATE
  PathResolver/PathResolver
  PUBLIC
  NtupleAnalysisUtils
  ${extra_dep} )

# External dependencies:
find_package( ROOT COMPONENTS Core MathCore Hist Graf Gpad File Tree Graf2d RIO )


atlas_add_library( IDPVMNtupleAnalysis
  IDPVMNtupleAnalysis/*.h Root/*.cxx
  PUBLIC_HEADERS IDPVMNtupleAnalysis
  INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} 
  PRIVATE_INCLUDE_DIRS Root 
  LINK_LIBRARIES ${ROOT_LIBRARIES} NtupleAnalysisUtils
  # ${extra_libs}
  )
  
  atlas_add_executable( CompareEfficiencies
    util/CompareEfficiencies.cxx
    INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} 
    LINK_LIBRARIES ${ROOT_LIBRARIES} IDPVMNtupleAnalysis )

  atlas_add_executable( CompareResolutions
    util/CompareResolutions.cxx
    INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} 
    LINK_LIBRARIES ${ROOT_LIBRARIES} IDPVMNtupleAnalysis )

  atlas_add_executable( ComparePulls
    util/ComparePulls.cxx
    INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} 
    LINK_LIBRARIES ${ROOT_LIBRARIES} IDPVMNtupleAnalysis )

  atlas_add_executable( IDPVMBinExtractor
    util/IDPVMBinExtractor.cxx
    INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} 
    LINK_LIBRARIES ${ROOT_LIBRARIES} IDPVMNtupleAnalysis )

  atlas_add_executable( ReadIDPVMTree
    util/ReadIDPVMTree.cxx
    INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} 
    LINK_LIBRARIES ${ROOT_LIBRARIES} IDPVMNtupleAnalysis )
