# FindBerkelium
# ------------
#
# Locate and configure the Berkelium library.
#
# The following variables can be set and are optional:
#
#    BERKELIUM_SRC_ROOT_FOLDER - When compiling with MSVC, if this cache variable is set
#                               the berkelium-default VS project build locations
#                               (vsprojects/Debug & vsprojects/Release) will be searched
#                               for libraries and binaries.
#
#    BERKELIUM_IMPORT_DIRS     - List of additional directories to be searched for
#                               imported .proto files. (New in CMake 2.8.8)
#
# Defines the following variables:
#
#    BERKELIUM_FOUND - Found the Berkelium library (libberkelium & header files)
#    BERKELIUM_INCLUDE_DIRS - Include directories for Berkelium
#    BERKELIUM_LIBRARIES - The berkelium libraries
#
# The following cache variables are also available to set or use:
#
#    BERKELIUM_LIBRARY - The berkelium library
#    BERKELIUM_INCLUDE_DIR - The include directory for berkelium
#    BERKELIUM_LIBRARY_DEBUG - The berkelium library (debug)
#
#   ====================================================================
#   Example:
#
#    find_package(Berkelium REQUIRED)
#    include_directories(${BERKELIUM_INCLUDE_DIRS})
#
#   ====================================================================

include(FindPackageHandleStandardArgs)

if(NOT BERKELIUM_SRC_ROOT_FOLDER)
    set(BERKELIUM_SRC_ROOT_FOLDER BERKELIUM_SRC_ROOT_FOLDER_NOT_FOUND 
        CACHE PATH "Berkelium root folder")
endif(NOT BERKELIUM_SRC_ROOT_FOLDER)

# The Berkelium library
find_library(BERKELIUM_LIBRARY
   NAMES berkelium
   PATHS ${BERKELIUM_SRC_ROOT_FOLDER}/lib)
mark_as_advanced(BERKELIUM_LIBRARY)

find_library(BERKELIUM_LIBRARY_DEBUG
   NAMES berkeliumd
   PATHS ${BERKELIUM_SRC_ROOT_FOLDER}/lib)
mark_as_advanced(BERKELIUM_LIBRARY_DEBUG)

if(NOT BERKELIUM_LIBRARY_DEBUG)
  # There is no debug library
  set(BERKELIUM_LIBRARY_DEBUG ${BERKELIUM_LIBRARY})
  set(BERKELIUM_LIBRARIES     ${BERKELIUM_LIBRARY})
else(NOT BERKELIUM_LIBRARY_DEBUG)
  # There IS a debug library
  set(BERKELIUM_LIBRARIES
      optimized ${BERKELIUM_LIBRARY}
      debug     ${BERKELIUM_LIBRARY_DEBUG}
  )
endif(NOT BERKELIUM_LIBRARY_DEBUG)

# Find the include directory
find_path(BERKELIUM_INCLUDE_DIR
    berkelium/Berkelium.hpp
    PATHS ${BERKELIUM_SRC_ROOT_FOLDER}/include
)
mark_as_advanced(BERKELIUM_INCLUDE_DIR)

find_package_handle_standard_args(BERKELIUM DEFAULT_MSG
    BERKELIUM_LIBRARY BERKELIUM_INCLUDE_DIR)

if(BERKELIUM_FOUND)
    set(BERKELIUM_INCLUDE_DIRS ${BERKELIUM_INCLUDE_DIR})
endif(BERKELIUM_FOUND)
