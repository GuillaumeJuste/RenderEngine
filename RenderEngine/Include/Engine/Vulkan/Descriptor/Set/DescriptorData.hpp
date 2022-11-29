#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETDATA
#define RENDERENGINE_DESCRIPTORSETDATA

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Texture/VkTexture.hpp"
#include "Engine/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"

namespace RenderEngine::Engine::Vulkan
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