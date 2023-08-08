# CMakeList.txt : CMake project for RenderEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# === Projects ===

project(RenderEngine)

add_library(RenderEngine STATIC)

file(GLOB_RECURSE SOURCES_PUBLIC "Include/*")
target_sources(RenderEngine PUBLIC ${SOURCES_PUBLIC})

file(GLOB_RECURSE SOURCES_PRIVATE "Source/*")
target_sources(RenderEngine PRIVATE ${SOURCES_PRIVATE})

# Input
file(GLOB_RECURSE SOURCES "Source/*")

# Add source to this project's executable.
target_include_directories(RenderEngine PUBLIC Include)
target_include_directories(RenderEngine PUBLIC Resources)
target_compile_definitions(RenderEngine PRIVATE NOMINMAX)

target_include_directories(RenderEngine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/Gitmodules)

# Add Mathlib library
target_link_libraries(RenderEngine PUBLIC Mathlib) 

# Add glfw library
target_link_libraries(RenderEngine PUBLIC glfw) 

# Add assimp library
target_link_libraries(RenderEngine PUBLIC assimp) 

# Add STB library
target_link_directories(RenderEngine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/Gitmodules/stb) 

#find_package(Vulkan REQUIRED)
#if(Vulkan_FOUND)
#	message(STATUS "Successfully located the Vulkan SDK: ${Vulkan_INCLUDE_DIRS}")	
#	include_directories(${Vulkan_INCLUDE_DIRS})
#	target_link_libraries(RenderEngine PUBLIC ${Vulkan_LIBRARIES} )
#else()
#	message(STATUS "Failed to locate Vulkan SDK")
#endif()

# Vulkan.
find_package(Vulkan REQUIRED FATAL_ERROR)
target_link_libraries(RenderEngine PUBLIC Vulkan::Vulkan)

#compile shaders
if(WIN32)
    add_custom_command(TARGET RenderEngine POST_BUILD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/RenderEngine/Resources/Shaders
        COMMAND cmd /C ${CMAKE_SOURCE_DIR}/RenderEngine/Resources/Shaders/CompileShader.bat)

    add_custom_command(TARGET RenderEngine POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_directory  
        ${CMAKE_SOURCE_DIR}/RenderEngine/Resources      
        $<TARGET_FILE_DIR:RenderEngine>/Resources) 
endif()