#pragma once

#ifndef RENDERENGINE_SHADERCOMPILEINFO
#define RENDERENGINE_SHADERCOMPILEINFO

#include <string>
#include <vector>
#include "AssetLoader/RawAsset/Shader/ShaderStage.hpp"

namespace Wrapper
{
	class ShaderCompileInfo
	{
	public:
		std::string path;

		std::string entrypoint = "main";

		std::string target;

		Loader::ShaderStage shaderStage;

		std::string standard = "2021";
		
		std::vector<std::string> defines;
	};
}

#endif