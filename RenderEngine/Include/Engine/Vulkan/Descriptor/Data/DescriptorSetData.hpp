#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETDATA
#define RENDERENGINE_DESCRIPTORSETDATA

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for DescriptorSet Initialization
	*/
	struct DescriptorSetData
	{
		std::vector<VkDescriptorBufferInfo> descriptorBufferInfos;

		DescriptorSetData() = default;
	};
}

#endif