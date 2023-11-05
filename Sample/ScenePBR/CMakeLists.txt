# Parse cpp files.
file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

# Add executable target.
add_executable (ScenePBR ${SOURCES})
target_include_directories(ScenePBR PUBLIC Resources)

# Add library dependencies.
target_link_libraries(ScenePBR PRIVATE RenderEngine)


if(WIN32)
    add_custom_command(TARGET ScenePBR POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_directory  
        ${CMAKE_SOURCE_DIR}/RenderEngine/Resources     
        $<TARGET_FILE_DIR:ScenePBR>/Resources/Engine) 

    add_custom_command(TARGET ScenePBR POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_directory  
        ${CMAKE_SOURCE_DIR}/Sample/ScenePBR/Resources     
        $<TARGET_FILE_DIR:ScenePBR>/Resources/Sample/ScenePBR) 

    add_custom_command(TARGET ScenePBR POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_directory  
        ${CMAKE_SOURCE_DIR}/ThirdParty/DirectXShaderCompiler/bin/x64    
        $<TARGET_FILE_DIR:ScenePBR>) 
endif()