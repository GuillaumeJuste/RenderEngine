add_subdirectory(Mathlib)
add_subdirectory(Glfw)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Skip building assimp tests.")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Do not build additionnal tools.")
set(ASSIMP_INSTALL OFF CACHE BOOL "Use assimp as a submodule")
add_subdirectory(Assimp)