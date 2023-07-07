# Parse cpp files.
file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

# Add executable target.
add_executable (PBRScene ${SOURCES})
target_include_directories(PBRScene PUBLIC Resources)

# Add library dependencies.
target_link_libraries(PBRScene PRIVATE RenderEngine)


if(WIN32)
    add_custom_command(TARGET PBRScene POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_directory  
        ${CMAKE_SOURCE_DIR}/RenderEngine/Resources     
        $<TARGET_FILE_DIR:PBRScene>/Resources/Engine) 

    add_custom_command(TARGET PBRScene POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_directory  
        ${CMAKE_SOURCE_DIR}/Sample/PBRScene/Resources     
        $<TARGET_FILE_DIR:PBRScene>/Resources/Sample/PBRScene) 
endif()