#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETIMAGEDATA
#define RENDERENGINE_DESCRIPTORSETIMAGEDATA

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for DescriptorSet Initialization
	*/
	struct DescriptorSetImageData
	{
		std::vector<VkDescriptorImageInfo> descriptorImageInfos;

		DescriptorSetImageData() = default;
	};
}

#endif