# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("OpenGL_Tutorial")

# Include sub-projects.
add_subdirectory ("glfw")
add_subdirectory ("glew")
add_subdirectory ("OpenGL_Tutorial")

target_include_directories (OpenGL_Tutorial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
target_include_directories (OpenGL_Tutorial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glew/include)