# CMakeList.txt : CMake project for RenderEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Input
file(GLOB_RECURSE SOURCES "Source/*")

# Add source to this project's executable.
add_executable (RenderEngine ${SOURCES})
target_include_directories(RenderEngine PUBLIC Include)
target_compile_definitions(RenderEngine PRIVATE GLFW_INCLUDE_VULKAN)
target_compile_definitions(RenderEngine PRIVATE VK_USE_PLATFORM_WIN32_KHR)
target_compile_definitions(RenderEngine PRIVATE GLFW_EXPOSE_NATIVE_WIN32)
target_compile_definitions(RenderEngine PRIVATE NOMINMAX)


# Add Mathlib library
add_library(Mathlib SHARED IMPORTED)
set_target_properties(Mathlib PROPERTIES
IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/ThirdParty/Mathlib/Lib/Mathlib.lib
IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/ThirdParty/Mathlib/Bin/Mathlib.dll
INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/ThirdParty/Mathlib/Include)
target_link_libraries(RenderEngine Mathlib) 

# Add glfw library
add_library(glfw SHARED IMPORTED)
set_target_properties(glfw PROPERTIES
IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/ThirdParty/glfw-3.3.7/lib-vc2019/glfw3.lib
IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/ThirdParty/glfw-3.3.7/lib-vc2019/glfw3.dll
INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/ThirdParty/glfw-3.3.7/include)
target_link_libraries(RenderEngine glfw) 


find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
	message(STATUS "Successfully located the Vulkan SDK: ${Vulkan_INCLUDE_DIRS}")	
	include_directories(${Vulkan_INCLUDE_DIRS})
	target_link_libraries(RenderEngine ${Vulkan_LIBRARIES} )
else()
	message(STATUS "Failed to locate Vulkan SDK")
endif()

add_custom_command(TARGET RenderEngine POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        ${CMAKE_SOURCE_DIR}/ThirdParty/Mathlib/Bin/Mathlib.dll      
        $<TARGET_FILE_DIR:RenderEngine>)                 