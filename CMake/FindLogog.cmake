# FindLogog
# ------------
#
# Locate and configure the Logog library.
#
# The following variables can be set and are optional:
#
#    LOGOG_SRC_ROOT_FOLDER - When compiling with MSVC, if this cache variable is set
#                               the logog-default VS project build locations
#                               (vsprojects/Debug & vsprojects/Release) will be searched
#                               for libraries and binaries.
#
#    LOGOG_IMPORT_DIRS     - List of additional directories to be searched for
#                               imported .proto files. (New in CMake 2.8.8)
#
# Defines the following variables:
#
#    LOGOG_FOUND - Found the Logog library (liblogog & header files)
#    LOGOG_INCLUDE_DIRS - Include directories for Logog
#    LOGOG_LIBRARIES - The logog libraries
#
# The following cache variables are also available to set or use:
#
#    LOGOG_LIBRARY - The logog library
#    LOGOG_INCLUDE_DIR - The include directory for logog
#    LOGOG_LIBRARY_DEBUG - The logog library (debug)
#
#   ====================================================================
#   Example:
#
#    find_package(Logog REQUIRED)
#    include_directories(${LOGOG_INCLUDE_DIRS})
#
#   ====================================================================

include(FindPackageHandleStandardArgs)

if(NOT LOGOG_SRC_ROOT_FOLDER)
    set(LOGOG_SRC_ROOT_FOLDER LOGOG_SRC_ROOT_FOLDER_NOT_FOUND
        CACHE PATH "Logog root folder")
endif(NOT LOGOG_SRC_ROOT_FOLDER)

# The Logog library
find_library(LOGOG_LIBRARY
   NAMES logog
   PATHS ${LOGOG_SRC_ROOT_FOLDER}/build/Release)
mark_as_advanced(LOGOG_LIBRARY)

find_library(LOGOG_LIBRARY_DEBUG
   NAMES logogd
   PATHS ${LOGOG_SRC_ROOT_FOLDER}/build/Debug)
mark_as_advanced(LOGOG_LIBRARY_DEBUG)

if(NOT LOGOG_LIBRARY_DEBUG)
  # There is no debug library
  set(LOGOG_LIBRARY_DEBUG ${LOGOG_LIBRARY})
  set(LOGOG_LIBRARIES     ${LOGOG_LIBRARY})
else(NOT LOGOG_LIBRARY_DEBUG)
  # There IS a debug library
  set(LOGOG_LIBRARIES
      optimized ${LOGOG_LIBRARY}
      debug     ${LOGOG_LIBRARY_DEBUG}
  )
endif(NOT LOGOG_LIBRARY_DEBUG)

# Find the include directory
find_path(LOGOG_INCLUDE_DIR
    logog.hpp
    PATHS ${LOGOG_SRC_ROOT_FOLDER}/include
)
mark_as_advanced(LOGOG_INCLUDE_DIR)

find_package_handle_standard_args(LOGOG DEFAULT_MSG
    LOGOG_LIBRARY LOGOG_INCLUDE_DIR)

if(LOGOG_FOUND)
    set(LOGOG_INCLUDE_DIRS ${LOGOG_INCLUDE_DIR})
endif(LOGOG_FOUND)
