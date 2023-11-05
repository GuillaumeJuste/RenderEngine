add_library(DirectXShaderCompiler INTERFACE)

target_include_directories(DirectXShaderCompiler INTERFACE include)

if(WIN32)
	target_link_directories(DirectXShaderCompiler INTERFACE bin/x64)
	target_link_libraries(DirectXShaderCompiler INTERFACE dxcompiler.dll)

	# Do not directly link with dxil.dll (`dxil.lib missing` error).
	# Calling dxcompiler.dll will lookup for dxil.dll: copy file next to .exe
	#target_link_libraries(DirectXShaderCompiler INTERFACE dxil.dll)
endif()