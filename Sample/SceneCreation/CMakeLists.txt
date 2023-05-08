# Parse cpp files.
file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

# Add executable target.
add_executable (SceneCreation ${SOURCES})
target_include_directories(SceneCreation PUBLIC Resources)

# Add library dependencies.
target_link_libraries(SceneCreation PRIVATE RenderEngine)


if(WIN32)
    add_custom_command(TARGET SceneCreation POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_directory  
        ${CMAKE_SOURCE_DIR}/RenderEngine/Resources     
        $<TARGET_FILE_DIR:SceneCreation>/Resources/Engine) 

    add_custom_command(TARGET SceneCreation POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_directory  
        ${CMAKE_SOURCE_DIR}/Sample/SceneCreation/Resources     
        $<TARGET_FILE_DIR:SceneCreation>/Resources/Sample/SceneCreation) 
endif()