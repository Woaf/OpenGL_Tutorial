# CMakeList.txt : CMake project for OpenGL_Tutorial, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (OpenGL_Tutorial "main.cpp")

# TODO: Add tests and install targets if needed.
target_link_libraries (OpenGL_Tutorial 
	glfw
	libglew_static
)