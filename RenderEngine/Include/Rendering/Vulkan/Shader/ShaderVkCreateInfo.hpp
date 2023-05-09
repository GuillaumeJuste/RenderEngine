#pragma once

#ifndef RENDERENGINE_SHADERVKCREATEINFO
#define RENDERENGINE_SHADERVKCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include <fstream>

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for Shader Initialization
	*/
	struct ShaderVkCreateInfo
	{
		/// shader type
		VkShaderStageFlagBits shaderType;

		/// shader file path
		std::string shaderFilePath;

		/// logical device
		VkDevice device = VK_NULL_HANDLE;

		/// default constructor 
		ShaderVkCreateInfo() = default;

		/**
		 * @brief Parameter constructor
		 * @param _shaderType shader type
		 * @param _shaderFilePath shader file path
		 * @param _device logical device
		*/
		ShaderVkCreateInfo(VkShaderStageFlagBits _shaderType, std::string _shaderFilePath,const VkDevice& _device);
	};
}

#endif