# Basic drawing
add_executable(basic_draw 
    basic_draw.cpp
    ../mgl/_core/extension/default_shaders.cpp)
set_target_properties(basic_draw PROPERTIES COMPILE_DEFINITIONS MGL_USE_GLEW)
target_link_libraries(basic_draw GLEW)
target_include_directories(basic_draw PRIVATE mgl)