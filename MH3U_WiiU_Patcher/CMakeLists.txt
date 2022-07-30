cmake_minimum_required (VERSION 3.8)
project ("MH3U WiiU Patcher" VERSION 1.0)

# Break glass in case of misplaced libraries:
# set(Boost_DEBUG 1)

set(BOOST_ROOT "C:/boost/boost_1_78_0")
set(Boost_NAMESPACE "libboost")  # <- your compiled boost lib. files start with this
find_package(Boost 1.78.0 REQUIRED COMPONENTS
			 json program_options) 

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS}) 
	add_executable(MH3U_WiiU_Patcher
		"MH3U_WiiU_Patcher.cpp"
		"MH3U_WiiU_Patcher.h"
		"util.cpp"
		"util.h"
	)
	target_link_libraries(MH3U_WiiU_Patcher ${Boost_LIBRARIES})
	add_custom_command (
		TARGET MH3U_WiiU_Patcher POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/Patches $<TARGET_FILE_DIR:MH3U_WiiU_Patcher>/patches
	)
	add_custom_command (
		TARGET MH3U_WiiU_Patcher POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/Dep $<TARGET_FILE_DIR:MH3U_WiiU_Patcher>
	)

	if (CMAKE_VERSION VERSION_GREATER 3.12)
		set_property(TARGET MH3U_WiiU_Patcher PROPERTY CXX_STANDARD 20)
	endif()
endif()

# TODO: Add tests and install targets if needed.
