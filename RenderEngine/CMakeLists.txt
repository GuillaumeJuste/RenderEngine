# CMakeList.txt : CMake project for RenderEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Input
file(GLOB_RECURSE SOURCES "Source/*")

# Add source to this project's executable.
add_executable (RenderEngine ${SOURCES})
target_include_directories(RenderEngine PUBLIC Include)

# Add Mathlib library
add_library(Mathlib SHARED IMPORTED)
set_target_properties(Mathlib PROPERTIES
IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/ThirdParty/Mathlib/Lib/Mathlib.lib
IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/ThirdParty/Mathlib/Bin/Mathlib.dll
INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/ThirdParty/Mathlib/Include)
target_link_libraries(RenderEngine Mathlib) 

find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
	message(STATUS "Successfully located the Vulkan SDK: ${Vulkan_INCLUDE_DIRS}")	
	include_directories(${Vulkan_INCLUDE_DIRS})
	target_link_libraries(RenderEngine ${Vulkan_LIBRARIES} )
else()
	message(STATUS "Failed to locate Vulkan SDK")
endif()