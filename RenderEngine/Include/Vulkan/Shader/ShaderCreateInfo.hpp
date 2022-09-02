#pragma once

#ifndef RENDERENGINE_SHADERCREATEINFO
#define RENDERENGINE_SHADERCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include <fstream>

namespace RenderEngine::Vulkan
{
	/**
	* @brief struct holding data for Shader Initialization
	*/
	struct ShaderCreateInfo
	{
		/// shader type
		VkShaderStageFlagBits shaderType;

		/// shader file path
		std::string shaderFilePath;

		/// logical device
		VkDevice device = VK_NULL_HANDLE;

		/// default constructor 
		ShaderCreateInfo() = default;

		/**
		 * @brief Parameter constructor
		 * @param _shaderType shader type
		 * @param _shaderFilePath shader file path
		 * @param _device logical device
		*/
		ShaderCreateInfo(VkShaderStageFlagBits _shaderType, std::string _shaderFilePath,const VkDevice& _device);
	};
}

#endif