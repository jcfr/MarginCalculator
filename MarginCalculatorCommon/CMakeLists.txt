project(vtkMarginCalculatorCommon)

# --------------------------------------------------------------------------
# Configure headers
# --------------------------------------------------------------------------
set(configure_header_file vtkMarginCalculatorCommonConfigure.h)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/${configure_header_file}.in
  ${CMAKE_CURRENT_BINARY_DIR}/${configure_header_file}
  )

# --------------------------------------------------------------------------
# Sources
# --------------------------------------------------------------------------

SET (MarginCalculatorCommon_SRCS 
  MarginCalculatorCommon.cxx
  MarginCalculatorCommon.h
  )

SET (MarginCalculatorCommon_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${Slicer_Libs_INCLUDE_DIRS} CACHE INTERNAL "" FORCE)

# --------------------------------------------------------------------------
# Build the library
# --------------------------------------------------------------------------

set(lib_name ${PROJECT_NAME})
SET (MarginCalculatorCommon_LIBS ${MarginCalculatorCommon_LIBS}
  MRMLCore
  )

INCLUDE_DIRECTORIES( ${MarginCalculatorCommon_INCLUDE_DIRS} )
ADD_LIBRARY(${lib_name} ${MarginCalculatorCommon_SRCS})
TARGET_LINK_LIBRARIES( ${lib_name} ${MarginCalculatorCommon_LIBS} )

# Set loadable modules output
set_target_properties(${lib_name} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_BIN_DIR}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_LIB_DIR}"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_LIB_DIR}"
  )

IF (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" AND NOT WIN32)
  SET_TARGET_PROPERTIES(${lib_name} PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF()

# --------------------------------------------------------------------------
# Python wrapping
# --------------------------------------------------------------------------
if(VTK_WRAP_PYTHON AND BUILD_SHARED_LIBS)

  SlicerMacroPythonWrapModuleVTKLibrary(
    NAME ${PROJECT_NAME}
    SRCS ${MarginCalculatorCommon_SRCS}
    RELATIVE_PYTHON_DIR "."
    )

  # Export target
  set_property(GLOBAL APPEND PROPERTY Slicer_TARGETS ${PROJECT_NAME}Python)
endif()
