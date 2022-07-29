# CMakeList.txt : CMake project for MH3U_WiiU_Patcher, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("MH3U WiiU Patcher" VERSION 1.0)

# Add source to this project's executable.
add_executable (
	MH3U_WiiU_Patcher
	"MH3U_WiiU_Patcher.cpp"
	"MH3U_WiiU_Patcher.h"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MH3U_WiiU_Patcher PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
