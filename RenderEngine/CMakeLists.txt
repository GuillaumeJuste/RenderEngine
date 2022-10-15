# CMakeList.txt : CMake project for RenderEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Input
file(GLOB_RECURSE SOURCES "Source/*")

# Add source to this project's executable.
add_executable (RenderEngine ${SOURCES})
target_include_directories(RenderEngine PUBLIC Include)
target_include_directories(RenderEngine PUBLIC Resources)
target_compile_definitions(RenderEngine PRIVATE GLFW_INCLUDE_VULKAN)
target_compile_definitions(RenderEngine PRIVATE VK_USE_PLATFORM_WIN32_KHR)
target_compile_definitions(RenderEngine PRIVATE GLFW_EXPOSE_NATIVE_WIN32)
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


find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
	message(STATUS "Successfully located the Vulkan SDK: ${Vulkan_INCLUDE_DIRS}")	
	include_directories(${Vulkan_INCLUDE_DIRS})
	target_link_libraries(RenderEngine PUBLIC ${Vulkan_LIBRARIES} )
else()
	message(STATUS "Failed to locate Vulkan SDK")
endif()

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