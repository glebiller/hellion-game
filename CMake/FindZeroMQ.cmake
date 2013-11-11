# FindZeroMQ
# ------------
#
# Locate and configure the ZeroMQ library.
#
# The following variables can be set and are optional:
#
#    ZEROMQ_SRC_ROOT_FOLDER - When compiling with MSVC, if this cache variable is set
#                               the zmq-default VS project build locations
#                               (vsprojects/Debug & vsprojects/Release) will be searched
#                               for libraries and binaries.
#
#    ZEROMQ_IMPORT_DIRS     - List of additional directories to be searched for
#                               imported .proto files. (New in CMake 2.8.8)
#
# Defines the following variables:
#
#    ZEROMQ_FOUND - Found the ZeroMQ library (libzmq & header files)
#    ZEROMQ_INCLUDE_DIRS - Include directories for ZeroMQ
#    ZEROMQ_LIBRARIES - The zmq libraries
#
# The following cache variables are also available to set or use:
#
#    ZEROMQ_LIBRARY - The zmq library
#    ZEROMQ_INCLUDE_DIR - The include directory for zmq
#    ZEROMQ_LIBRARY_DEBUG - The zmq library (debug)
#
#   ====================================================================
#   Example:
#
#    find_package(ZeroMQ REQUIRED)
#    include_directories(${ZEROMQ_INCLUDE_DIRS})
#
#   ====================================================================

include(FindPackageHandleStandardArgs)

if(NOT ZEROMQ_SRC_ROOT_FOLDER)
    set(ZEROMQ_SRC_ROOT_FOLDER ZEROMQ_SRC_ROOT_FOLDER_NOT_FOUND
        CACHE PATH "ZeroMQ root folder")
endif(NOT ZEROMQ_SRC_ROOT_FOLDER)

# generate libraries with a "lib" prefix on Windows
if(MSVC)
    set(ZEROMQ_ORIG_FIND_LIBRARY_PREFIXES "${CMAKE_FIND_LIBRARY_PREFIXES}")
    set(CMAKE_FIND_LIBRARY_PREFIXES "lib" "")
endif(MSVC)

# The ZeroMQ library
find_library(ZEROMQ_LIBRARY
   NAMES zmq
   PATHS ${ZEROMQ_SRC_ROOT_FOLDER}/lib/Win32)
mark_as_advanced(ZEROMQ_LIBRARY)

find_library(ZEROMQ_LIBRARY_DEBUG
   NAMES zmq_d
   PATHS ${ZEROMQ_SRC_ROOT_FOLDER}/lib/Win32)
mark_as_advanced(ZEROMQ_LIBRARY_DEBUG)

if(NOT ZEROMQ_LIBRARY_DEBUG)
  # There is no debug library
  set(ZEROMQ_LIBRARY_DEBUG ${ZEROMQ_LIBRARY})
  set(ZEROMQ_LIBRARIES     ${ZEROMQ_LIBRARY})
else(NOT ZEROMQ_LIBRARY_DEBUG)
  # There IS a debug library
  set(ZEROMQ_LIBRARIES
      optimized ${ZEROMQ_LIBRARY}
      debug     ${ZEROMQ_LIBRARY_DEBUG}
  )
endif(NOT ZEROMQ_LIBRARY_DEBUG)

# Restore original find library prefixes
if(MSVC)
    set(CMAKE_FIND_LIBRARY_PREFIXES "${ZEROMQ_ORIG_FIND_LIBRARY_PREFIXES}")
endif(MSVC)

# Find the include directory
find_path(ZEROMQ_INCLUDE_DIR
    zmq.hpp
    PATHS ${ZEROMQ_SRC_ROOT_FOLDER}/include
)
mark_as_advanced(ZEROMQ_INCLUDE_DIR)

find_package_handle_standard_args(ZEROMQ DEFAULT_MSG
    ZEROMQ_LIBRARY ZEROMQ_INCLUDE_DIR)

if(ZEROMQ_FOUND)
    set(ZEROMQ_INCLUDE_DIRS ${ZEROMQ_INCLUDE_DIR})
endif(ZEROMQ_FOUND)
