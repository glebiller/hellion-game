# FindHavok
# ------------
#
# Locate and configure the Havok library.
#
# The following variables can be set and are optional:
#
#    HAVOK_SRC_ROOT_FOLDER - When compiling with MSVC, if this cache variable is set
#                               the havok-default VS project build locations
#                               (vsprojects/Debug & vsprojects/Release) will be searched
#                               for libraries and binaries.
#
#    HAVOK_IMPORT_DIRS     - List of additional directories to be searched for
#                               imported .proto files. (New in CMake 2.8.8)
#
# Defines the following variables:
#
#    HAVOK_FOUND - Found the Havok library (libhavok & header files)
#    HAVOK_INCLUDE_DIRS - Include directories for Havok
#    HAVOK_LIBRARIES - The havok libraries
#
# The following cache variables are also available to set or use:
#
#    HAVOK_LIBRARY - The havok library
#    HAVOK_INCLUDE_DIR - The include directory for havok
#    HAVOK_LIBRARY_DEBUG - The havok library (debug)
#
#   ====================================================================
#   Example:
#
#    find_package(HAVOK REQUIRED)
#    include_directories(${HAVOK_INCLUDE_DIRS})
#
#   ====================================================================

include(FindPackageHandleStandardArgs)

macro(havok_find_component component)
    string(TOUPPER ${component} COMPONENT_U)
    
    find_library(HAVOK_${COMPONENT_U}_LIBRARY
       NAMES "${component}"
       PATHS "${HAVOK_SRC_ROOT_FOLDER}/Lib/win32_vs2012_win8/release_dll")
    find_library(HAVOK_${COMPONENT_U}_LIBRARY_DEBUG
       NAMES "${component}" 
       PATHS "${HAVOK_SRC_ROOT_FOLDER}/Lib/win32_vs2012_win8/debug_dll")
    mark_as_advanced(HAVOK_${COMPONENT_U}_LIBRARY HAVOK_${COMPONENT_U}_LIBRARY_DEBUG)
    
    if(NOT HAVOK_${COMPONENT_U}_LIBRARY_DEBUG)
        # There is no debug library
        list(APPEND HAVOK_LIBRARY_DEBUG ${HAVOK_${COMPONENT_U}_LIBRARY})
        list(APPEND HAVOK_LIBRARIES ${HAVOK_${COMPONENT_U}_LIBRARY})
    else(NOT HAVOK_${COMPONENT_U}_LIBRARY_DEBUG)
        # There IS a debug library
        list(APPEND HAVOK_LIBRARIES
            optimized ${HAVOK_${COMPONENT_U}_LIBRARY}
            debug     ${HAVOK_${COMPONENT_U}_LIBRARY_DEBUG}
        )
    endif(NOT HAVOK_${COMPONENT_U}_LIBRARY_DEBUG)
    
    find_package_handle_standard_args("HAVOK_${COMPONENT_U}" DEFAULT_MSG
        "HAVOK_${COMPONENT_U}_LIBRARY")
endmacro()

if(NOT HAVOK_SRC_ROOT_FOLDER)
    set(HAVOK_SRC_ROOT_FOLDER HAVOK_SRC_ROOT_FOLDER_NOT_FOUND
        CACHE PATH "HAVOK root folder")
endif(NOT HAVOK_SRC_ROOT_FOLDER)

# The HAVOK library
find_library(HAVOK_LIBRARY
   NAMES "hkBase"
   PATHS "${HAVOK_SRC_ROOT_FOLDER}/Lib/win32_vs2012_win8/release_dll")
find_library(HAVOK_LIBRARY_DEBUG
   NAMES "hkBase"
   PATHS "${HAVOK_SRC_ROOT_FOLDER}/Lib/win32_vs2012_win8/debug_dll")
mark_as_advanced(HAVOK_LIBRARY HAVOK_LIBRARY_DEBUG)

if(NOT HAVOK_LIBRARY_DEBUG)
  # There is no debug library
  set(HAVOK_LIBRARY_DEBUG ${HAVOK_LIBRARY})
  set(HAVOK_LIBRARIES     ${HAVOK_LIBRARY})
else(NOT HAVOK_LIBRARY_DEBUG)
  # There IS a debug library
  set(HAVOK_LIBRARIES
      optimized ${HAVOK_LIBRARY}
      debug     ${HAVOK_LIBRARY_DEBUG}
  )
endif(NOT HAVOK_LIBRARY_DEBUG)

# Find the include directory
find_path(HAVOK_INCLUDE_DIR
    "Common/Base/hkBase.h"
    PATHS ${HAVOK_SRC_ROOT_FOLDER}/Source
)
mark_as_advanced(HAVOK_INCLUDE_DIR)

find_package_handle_standard_args(HAVOK DEFAULT_MSG
    HAVOK_LIBRARY HAVOK_INCLUDE_DIR)

if(HAVOK_FOUND)
    set(HAVOK_INCLUDE_DIRS ${HAVOK_INCLUDE_DIR})
    
    if(Havok_FIND_COMPONENTS)
        foreach(COMPONENT ${Havok_FIND_COMPONENTS})
            havok_find_component(${COMPONENT})
        endforeach()
    endif(Havok_FIND_COMPONENTS)
endif(HAVOK_FOUND)
