cmake_minimum_required(VERSION 2.8.4)

# Project name
project(mgl)

# Set version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(GNUInstallDirs)

install(DIRECTORY mgl DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR})