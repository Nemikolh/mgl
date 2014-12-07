cmake_minimum_required(VERSION 2.8.4)

# Project name
project(mgl)

# Set version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(GNUInstallDirs)

# Set flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(MGL_DEBUG_FILES mgl/_debug/_debug.cpp)

# Option for MGL_DEBUG_VERSION
option(MGL_DEBUG_VERSION "MGL compile debug version." ON)
option(MGL_DEBUG_USE_GLEW "MGL compile debug version using GLEW." ON)
option(MGL_DEBUG_USE_ES "MGL compile debug version using ES." OFF)

if(MGL_DEBUG_VERSION)
    include_directories(mgl)
    add_library(mgl-debug ${MGL_DEBUG_FILES})
    if(MGL_DEBUG_USE_ES)
        set_target_properties( mgl-debug PROPERTIES COMPILE_FLAGS "-DMGL_USE_ES2" )
    elseif(MGL_DEBUG_USE_GLEW)
        set_target_properties( mgl-debug PROPERTIES COMPILE_FLAGS "-DMGL_USE_GLEW" )
    endif()
endif()


# Install files in appropriate path
install(mgl-debug LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
install(DIRECTORY mgl DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR})