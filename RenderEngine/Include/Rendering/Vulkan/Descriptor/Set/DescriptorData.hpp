#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETDATA
#define RENDERENGINE_DESCRIPTORSETDATA

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Texture/VkTexture.hpp"
#include "Rendering/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"

namespace RenderEngine::Rendering
{
	struct DescriptorData
	{
		VkDescriptorType descriptorType;
		VkShaderStageFlags stageFlags;
		uint32_t binding;

		VkTexture* texture = nullptr;
		DescriptorBuffer* buffer = nullptr;

		DescriptorData() = default;

		bool operator==(const DescriptorData& _rhs);
	};
}

#endif