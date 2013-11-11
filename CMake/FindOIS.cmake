# FindOIS
# ------------
#
# Locate and configure the OIS library.
#
# The following variables can be set and are optional:
#
#    OIS_SRC_ROOT_FOLDER - When compiling with MSVC, if this cache variable is set
#                               the ois-default VS project build locations
#                               (vsprojects/Debug & vsprojects/Release) will be searched
#                               for libraries and binaries.
#
#    OIS_IMPORT_DIRS     - List of additional directories to be searched for
#                               imported .proto files. (New in CMake 2.8.8)
#
# Defines the following variables:
#
#    OIS_FOUND - Found the OIS library (libois & header files)
#    OIS_INCLUDE_DIRS - Include directories for OIS
#    OIS_LIBRARIES - The ois libraries
#
# The following cache variables are also available to set or use:
#
#    OIS_LIBRARY - The ois library
#    OIS_INCLUDE_DIR - The include directory for ois
#    OIS_LIBRARY_DEBUG - The ois library (debug)
#
#   ====================================================================
#   Example:
#
#    find_package(OIS REQUIRED)
#    include_directories(${OIS_INCLUDE_DIRS})
#
#   ====================================================================

include(FindPackageHandleStandardArgs)

if(NOT OIS_SRC_ROOT_FOLDER)
    set(OIS_SRC_ROOT_FOLDER OIS_SRC_ROOT_FOLDER_NOT_FOUND
        CACHE PATH "OIS root folder")
endif(NOT OIS_SRC_ROOT_FOLDER)

# The OIS library
find_library(OIS_LIBRARY
   NAMES "OIS"
   PATHS ${OIS_SRC_ROOT_FOLDER}/lib/Release)
mark_as_advanced(OIS_LIBRARY)

find_library(OIS_LIBRARY_DEBUG
   NAMES "OIS_d"
   PATHS ${OIS_SRC_ROOT_FOLDER}/lib/Debug)
mark_as_advanced(OIS_LIBRARY_DEBUG)

if(NOT OIS_LIBRARY_DEBUG)
  # There is no debug library
  set(OIS_LIBRARY_DEBUG ${OIS_LIBRARY})
  set(OIS_LIBRARIES     ${OIS_LIBRARY})
else(NOT OIS_LIBRARY_DEBUG)
  # There IS a debug library
  set(OIS_LIBRARIES
      optimized ${OIS_LIBRARY}
      debug     ${OIS_LIBRARY_DEBUG}
  )
endif(NOT OIS_LIBRARY_DEBUG)

# Find the include directory
find_path(OIS_INCLUDE_DIR
    OIS.h
    PATHS ${OIS_SRC_ROOT_FOLDER}/include/OIS
)
mark_as_advanced(OIS_INCLUDE_DIR)

find_package_handle_standard_args(OIS DEFAULT_MSG
    OIS_LIBRARY OIS_INCLUDE_DIR)

if(OIS_FOUND)
    set(OIS_INCLUDE_DIRS ${OIS_INCLUDE_DIR})
endif(OIS_FOUND)
