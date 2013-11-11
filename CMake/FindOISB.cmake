# FindOISB
# ------------
#
# Locate and configure the OISB library.
#
# The following variables can be set and are optional:
#
#    OISB_SRC_ROOT_FOLDER - When compiling with MSVC, if this cache variable is set
#                               the oisb-default VS project build locations
#                               (vsprojects/Debug & vsprojects/Release) will be searched
#                               for libraries and binaries.
#
#    OISB_IMPORT_DIRS     - List of additional directories to be searched for
#                               imported .proto files. (New in CMake 2.8.8)
#
# Defines the following variables:
#
#    OISB_FOUND - Found the OISB library (liboisb & header files)
#    OISB_INCLUDE_DIRS - Include directories for OISB
#    OISB_LIBRARIES - The oisb libraries
#
# The following cache variables are also available to set or use:
#
#    OISB_LIBRARY - The oisb library
#    OISB_INCLUDE_DIR - The include directory for oisb
#    OISB_LIBRARY_DEBUG - The oisb library (debug)
#
#   ====================================================================
#   Example:
#
#    find_package(OISB REQUIRED)
#    include_directories(${OISB_INCLUDE_DIRS})
#
#   ====================================================================

include(FindPackageHandleStandardArgs)

if(NOT OISB_SRC_ROOT_FOLDER)
    set(OISB_SRC_ROOT_FOLDER OISB_SRC_ROOT_FOLDER_NOT_FOUND
        CACHE PATH "OISB root folder")
endif(NOT OISB_SRC_ROOT_FOLDER)

# The OISB library
find_library(OISB_LIBRARY
   NAMES OISB
   PATHS ${OISB_SRC_ROOT_FOLDER}/Build/Release)
mark_as_advanced(OISB_LIBRARY)

find_library(OISB_LIBRARY_DEBUG
   NAMES OISB
   PATHS ${OISB_SRC_ROOT_FOLDER}/Build/Debug)
mark_as_advanced(OISB_LIBRARY_DEBUG)

if(NOT OISB_LIBRARY_DEBUG)
  # There is no debug library
  set(OISB_LIBRARY_DEBUG ${OISB_LIBRARY})
  set(OISB_LIBRARIES     ${OISB_LIBRARY})
else(NOT OISB_LIBRARY_DEBUG)
  # There IS a debug library
  set(OISB_LIBRARIES
      optimized ${OISB_LIBRARY}
      debug     ${OISB_LIBRARY_DEBUG}
  )
endif(NOT OISB_LIBRARY_DEBUG)

# Find the include directory
find_path(OISB_INCLUDE_DIR
    OISB.h
    PATHS ${OISB_SRC_ROOT_FOLDER}/Source
)
mark_as_advanced(OISB_INCLUDE_DIR)

find_package_handle_standard_args(OISB DEFAULT_MSG
    OISB_LIBRARY OISB_INCLUDE_DIR)

if(OISB_FOUND)
    set(OISB_INCLUDE_DIRS ${OISB_INCLUDE_DIR})
endif(OISB_FOUND)
