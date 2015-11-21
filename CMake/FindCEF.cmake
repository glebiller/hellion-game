# FindChromium Embedded Framework
# ------------
#
# Locate and configure the Chromium Embedded Framework library.
#
# The following variables can be set and are optional:
#
#    CEF_SRC_ROOT_FOLDER - When compiling with MSVC, if this cache variable is set
#                               the chromium embedded framework-default VS project build locations
#                               (vsprojects/Debug & vsprojects/Release) will be searched
#                               for libraries and binaries.
#
#    CEF_IMPORT_DIRS     - List of additional directories to be searched for
#                               imported .proto files. (New in CMake 2.8.8)
#
# Defines the following variables:
#
#    CEF_FOUND - Found the Chromium Embedded Framework library (libchromium embedded framework & header files)
#    CEF_INCLUDE_DIRS - Include directories for Chromium Embedded Framework
#    CEF_LIBRARIES - The chromium embedded framework libraries
#
# The following cache variables are also available to set or use:
#
#    CEF_LIBRARY - The chromium embedded framework library
#    CEF_INCLUDE_DIR - The include directory for chromium embedded framework
#    CEF_LIBRARY_DEBUG - The chromium embedded framework library (debug)
#
#   ====================================================================
#   Example:
#
#    find_package(Chromium Embedded Framework REQUIRED)
#    include_directories(${CEF_INCLUDE_DIRS})
#
#   ====================================================================

include(SelectLibraryConfigurations)
include(FindPackageHandleStandardArgs)

if (NOT CEF_SRC_ROOT_FOLDER)
    set(CEF_SRC_ROOT_FOLDER CEF_SRC_ROOT_FOLDER_NOT_FOUND
            CACHE PATH "Chromium Embedded Framework root folder")
endif (NOT CEF_SRC_ROOT_FOLDER)

# The Chromium Embedded Framework library
find_library(CEF_LIBRARY_RELEASE
        NAMES libcef "Chromium Embedded Framework"
        PATHS ${CEF_SRC_ROOT_FOLDER}/Release)
mark_as_advanced(CEF_LIBRARY_RELEASE)

find_library(CEF_DLL_WRAPPER_RELEASE
        NAMES libcef_dll_wrapper
        PATHS ${CEF_SRC_ROOT_FOLDER}/Release)
mark_as_advanced(CEF_DLL_WRAPPER_RELEASE)

find_library(CEF_LIBRARY_DEBUG
        NAMES libcef "Chromium Embedded Framework"
        PATHS ${CEF_SRC_ROOT_FOLDER}/Debug)
mark_as_advanced(CEF_LIBRARY_DEBUG)

find_library(CEF_DLL_WRAPPER_DEBUG
        NAMES libcef_dll_wrapper
        PATHS ${CEF_SRC_ROOT_FOLDER}/Debug)
mark_as_advanced(CEF_DLL_WRAPPER_DEBUG)

if (NOT CEF_LIBRARY_DEBUG AND NOT CEF_DLL_WRAPPER_RELEASE)
    # There is no debug library
    set(CEF_LIBRARIES_DEBUG ${CEF_LIBRARY_RELEASE} ${CEF_DLL_WRAPPER_RELEASE})
    set(CEF_LIBRARIES ${CEF_LIBRARY_RELEASE} ${CEF_DLL_WRAPPER_RELEASE})
else(NOT CEF_LIBRARY_DEBUG AND NOT CEF_DLL_WRAPPER_RELEASE)
    # There IS a debug library
    set(CEF_LIBRARIES
            optimized ${CEF_LIBRARY_RELEASE} ${CEF_DLL_WRAPPER_RELEASE}
            debug ${CEF_LIBRARY_DEBUG} ${CEF_DLL_WRAPPER_DEBUG}
    )
endif (NOT CEF_LIBRARY_DEBUG AND NOT CEF_DLL_WRAPPER_RELEASE)

# Find the include directory
find_path(CEF_INCLUDE_DIR "include/cef_version.h"
        PATHS ${CEF_SRC_ROOT_FOLDER}
)
mark_as_advanced(CEF_INCLUDE_DIR)

find_package_handle_standard_args(CEF DEFAULT_MSG
        CEF_LIBRARY_RELEASE CEF_INCLUDE_DIR)

if (CEF_FOUND)
    set(CEF_INCLUDE_DIRS ${CEF_INCLUDE_DIR})
endif (CEF_FOUND)
