#pragma once

#ifndef RENDERENGINE_BASEDESCRIPTORSETDATA
#define RENDERENGINE_BASEDESCRIPTORSETDATA

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct BaseDescriptorSetData
	{
		VkDescriptorType descriptorType;
		VkShaderStageFlags stageFlags;
		uint32_t binding;

		BaseDescriptorSetData() = default;
	};
}

#endif