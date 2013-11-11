# FindOgre
# ------------
#
# Locate and configure the Ogre library.
#
# The following variables can be set and are optional:
#
#    OGRE_SRC_ROOT_FOLDER - When compiling with MSVC, if this cache variable is set
#                               the ogre-default VS project build locations
#                               (vsprojects/Debug & vsprojects/Release) will be searched
#                               for libraries and binaries.
#
#    OGRE_IMPORT_DIRS     - List of additional directories to be searched for
#                               imported .proto files. (New in CMake 2.8.8)
#
# Defines the following variables:
#
#    OGRE_FOUND - Found the Ogre library (libogre & header files)
#    OGRE_INCLUDE_DIRS - Include directories for Ogre
#    OGRE_LIBRARIES - The ogre libraries
#
# The following cache variables are also available to set or use:
#
#    OGRE_LIBRARY - The ogre library
#    OGRE_INCLUDE_DIR - The include directory for ogre
#    OGRE_LIBRARY_DEBUG - The ogre library (debug)
#
#   ====================================================================
#   Example:
#
#    find_package(Ogre REQUIRED)
#    include_directories(${OGRE_INCLUDE_DIRS})
#
#   ====================================================================

include(FindPackageHandleStandardArgs)

macro(ogre_find_component component)
    string(TOUPPER ${component} COMPONENT_U)
    
    find_path(OGRE_${COMPONENT_U}_INCLUDE_DIR NAMES "Ogre${component}.h" "Ogre${component}*.h"
        HINTS ${OGRE_INCLUDE_DIRS} ${OGRE_PREFIX_SOURCE} 
        PATH_SUFFIXES ${component} OGRE/${component} Components/${component}/include)
    find_library(OGRE_${COMPONENT_U}_LIBRARY
       NAMES "Ogre${component}"
       PATHS "${OGRE_SRC_ROOT_FOLDER}/lib/Release")
    find_library(OGRE_${COMPONENT_U}_LIBRARY_DEBUG
       NAMES "Ogre${component}_d" 
       PATHS "${OGRE_SRC_ROOT_FOLDER}/lib/Debug")
    mark_as_advanced(OGRE_${COMPONENT_U}_LIBRARY OGRE_${COMPONENT_U}_LIBRARY_DEBUG)
    
    if(NOT OGRE_${COMPONENT_U}_LIBRARY_DEBUG)
        # There is no debug library
        list(APPEND OGRE_LIBRARY_DEBUG ${OGRE_${COMPONENT_U}_LIBRARY})
        list(APPEND OGRE_LIBRARIES ${OGRE_${COMPONENT_U}_LIBRARY})
    else(NOT OGRE_${COMPONENT_U}_LIBRARY_DEBUG)
        # There IS a debug library
        list(APPEND OGRE_LIBRARIES
            optimized ${OGRE_${COMPONENT_U}_LIBRARY}
            debug     ${OGRE_${COMPONENT_U}_LIBRARY_DEBUG}
        )
    endif(NOT OGRE_${COMPONENT_U}_LIBRARY_DEBUG)
    
    find_package_handle_standard_args("OGRE_${COMPONENT_U}" DEFAULT_MSG
        "OGRE_${COMPONENT_U}_LIBRARY" "OGRE_${COMPONENT_U}_INCLUDE_DIR")
    
    if (OGRE_${COMPONENT_U}_FOUND)
        list(APPEND OGRE_INCLUDE_DIRS ${OGRE_${COMPONENT_U}_INCLUDE_DIR})
    endif()
endmacro()

if(NOT OGRE_SRC_ROOT_FOLDER)
    set(OGRE_SRC_ROOT_FOLDER OGRE_SRC_ROOT_FOLDER_NOT_FOUND
        CACHE PATH "Ogre root folder")
endif(NOT OGRE_SRC_ROOT_FOLDER)

# The Ogre library
find_library(OGRE_LIBRARY
   NAMES OgreMain
   PATHS ${OGRE_SRC_ROOT_FOLDER}/lib/Release)
mark_as_advanced(OGRE_LIBRARY)

find_library(OGRE_LIBRARY_DEBUG
   NAMES OgreMain_d
   PATHS ${OGRE_SRC_ROOT_FOLDER}/lib/Debug)
mark_as_advanced(OGRE_LIBRARY_DEBUG)

if(NOT OGRE_LIBRARY_DEBUG)
  # There is no debug library
  set(OGRE_LIBRARY_DEBUG ${OGRE_LIBRARY})
  set(OGRE_LIBRARIES     ${OGRE_LIBRARY})
else(NOT OGRE_LIBRARY_DEBUG)
  # There IS a debug library
  set(OGRE_LIBRARIES
      optimized ${OGRE_LIBRARY}
      debug     ${OGRE_LIBRARY_DEBUG}
  )
endif(NOT OGRE_LIBRARY_DEBUG)

# Find the include directory
find_path(OGRE_INCLUDE_DIR
    ogre.h
    PATHS ${OGRE_SRC_ROOT_FOLDER}/include/OGRE
)
mark_as_advanced(OGRE_INCLUDE_DIR)

find_package_handle_standard_args(OGRE DEFAULT_MSG
    OGRE_LIBRARY OGRE_INCLUDE_DIR)

if(OGRE_FOUND)
    list(APPEND OGRE_INCLUDE_DIRS ${OGRE_INCLUDE_DIR})
    
    if(Ogre_FIND_COMPONENTS)
        foreach(COMPONENT ${Ogre_FIND_COMPONENTS})
            ogre_find_component(${COMPONENT})
        endforeach()
    endif(Ogre_FIND_COMPONENTS)
endif(OGRE_FOUND)
