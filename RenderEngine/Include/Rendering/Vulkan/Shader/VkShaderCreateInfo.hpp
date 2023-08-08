#pragma once

#ifndef RENDERENGINE_SHADERVKCREATEINFO
#define RENDERENGINE_SHADERVKCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "AssetLoader/RawAsset/SHader/RawShader.hpp"

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for Shader Initialization
	*/
	struct VkShaderCreateInfo
	{
		Loader::RawShader rawShader;

		/// logical device
		VkDevice device = VK_NULL_HANDLE;

		/// default constructor 
		VkShaderCreateInfo() = default;
	};
}

#endif