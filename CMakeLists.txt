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


atlas_add_library( IDPVMNtupleAnalysisLib
  IDPVMNtupleAnalysis/*.h Root/*.cxx
  PUBLIC_HEADERS IDPVMNtupleAnalysis
  INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} 
  PRIVATE_INCLUDE_DIRS Root 
  LINK_LIBRARIES ${ROOT_LIBRARIES} NtupleAnalysisUtils
  # ${extra_libs}
  )

file(GLOB_RECURSE files "util/*.cxx")
foreach(_exeFile ${files})
    get_filename_component(_theExec ${_exeFile} NAME_WE)
    get_filename_component(_theLoc ${_exeFile} DIRECTORY)
    if(${_theLoc} MATCHES "DoNotBuild")
    else()
      atlas_add_executable( ${_theExec}
      ${_exeFile}
      INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} 
      LINK_LIBRARIES ${ROOT_LIBRARIES} IDPVMNtupleAnalysisLib )
    endif()
endforeach()