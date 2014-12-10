set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

# Depencencies
find_package(SFML 2.0 REQUIRED system window graphics)
find_package(GLM REQUIRED)

include_directories(${SFML_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

# Basic drawing
add_executable(basic_draw 
    basic_draw.cpp
    ../mgl/_core/extension/default_shaders.cpp)
set_target_properties(basic_draw PROPERTIES COMPILE_DEFINITIONS MGL_USE_GLEW)
target_link_libraries(basic_draw 
    ${SFML_LIBRARIES}
    mgl-debug 
    GLEW
    GL)