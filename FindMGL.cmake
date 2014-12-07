# FindMGL                  - attempts to locate the mgl library.
#
# This module defines the following variables (on success):
#     MGL_INCLUDE_DIRS     - where to find mgl headers
#     MGL_FOUND            - if the library was successfully located
#
# It is trying a few standard installation locations, but can be customized
# with the following variable:
#     MGL_ROOT_DIR         - root directory of a mgl installation
#                            Headers are expected to be found in:
#                                <MGL_ROOT_DIR>/mgl/
#                            This variable can either be a cmake or environment
#                            variable.
#
#


# default search dirs
set(_MGL_HEADER_SEARCH_DIRS
    "/usr/include"
    "/usr/local/include")
    
# check environment variable
set(_MGL_ENV_ROOT_DIR "$ENV{GLM_ROOT_DIR}")

if(NOT MGL_ROOT_DIR AND _MGL_ENV_ROOT_DIR)
    set(MGL_ROOT_DIR "${_MGL_ENV_ROOT_DIR}")
endif()

if(MGL_ROOT_DIR)
    set(_MGL_HEADER_SEARCH_DIRS "${MGL_ROOT_DIR}"
                                ${_MGL_HEADER_SEARCH_DIRS})
endif()

# locate header
find_path(MGL_INCLUDE_DIR "mgl/_setup.hpp" PATHS ${_MGL_HEADER_SEARCH_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MGL DEFAULT_MSG MGL_INCLUDE_DIR)

if(MGL_FOUND)
    set(MGL_INCLUDE_DIRS "${MGL_INCLUDE_DIR}")
    message(STATUS "MGL_INCLUDE_DIR = ${MGL_INCLUDE_DIR}")
endif()