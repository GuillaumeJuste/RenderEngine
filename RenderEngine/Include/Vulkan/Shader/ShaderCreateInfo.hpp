#pragma once

#ifndef RENDERENGINE_SHADERCREATEINFO
#define RENDERENGINE_SHADERCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include <fstream>

namespace RenderEngine::Vulkan
{
	struct ShaderCreateInfo
	{
		VkShaderStageFlagBits shaderType;
		std::string shaderFilePath;
		VkDevice device = VK_NULL_HANDLE;

		ShaderCreateInfo() = default;
		ShaderCreateInfo(VkShaderStageFlagBits _shaderType, std::string _shaderFilePath,const VkDevice& _device);
	};
}

#endif