#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETBUFFERDATA
#define RENDERENGINE_DESCRIPTORSETBUFFERDATA

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for DescriptorSet Initialization
	*/
	struct DescriptorSetBufferData
	{
		std::vector<VkDescriptorBufferInfo> descriptorBufferInfos;

		DescriptorSetBufferData() = default;
	};
}

#endif