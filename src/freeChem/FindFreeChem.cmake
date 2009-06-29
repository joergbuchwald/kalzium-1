# FREECHEM_FOUND - system has lib conversion
# FREECHEM_INCLUDE_DIR - conversion include directory
# FREECHEM_LIBRARIES - library needed to use conversion

if (FREECHEM_INCLUDE_DIR AND FREECHEM_LIBRARIES)
   set(FREECHEM_FIND_QUIETLY TRUE)
endif (FREECHEM_INCLUDE_DIR AND FREECHEM_LIBRARIES)

find_path(FREECHEM_INCLUDE_DIR NAMES chemistry.h PATH_SUFFIXES freeChem)
find_library(FREECHEM_LIBRARIES NAMES freeChem)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FREECHEM DEFAULT_MSG FREECHEM_INCLUDE_DIR FREECHEM_LIBRARIES)