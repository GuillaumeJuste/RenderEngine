#pragma once

#ifndef RENDERENGINE_SHADERVKCREATEINFO
#define RENDERENGINE_SHADERVKCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "ResourceManager/Assets/Shader/RawShader.hpp"

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for Shader Initialization
	*/
	struct VkShaderCreateInfo
	{
		RenderEngine::Assets::RawShader rawShader;

		/// logical device
		VkDevice device = VK_NULL_HANDLE;

		/// default constructor 
		VkShaderCreateInfo() = default;
	};
}

#endif