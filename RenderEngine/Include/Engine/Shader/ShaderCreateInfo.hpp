#pragma once

#ifndef RENDERENGINE_SHADERCREATEINFO
#define RENDERENGINE_SHADERCREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"
#include <fstream>

namespace RenderEngine
{
	enum class ShaderType
	{
		VERTEX_SHADER = 0x00000001,
		FRAGMENT_SHADER = 0x00000010,
		COMPUTE_SHADER = 0x00000020,
		GEMOETRY_SHADER = 0x00000008,
	};

	struct ShaderCreateInfo
	{
		ShaderType shaderType = ShaderType::VERTEX_SHADER;
		std::string shaderFilePath;
		VkDevice device ;

		ShaderCreateInfo() = default;
	};
}

#endif