#pragma once

#ifndef RENDERENGINE_VKGAMEOBJECTCREATEINFO
#define RENDERENGINE_VKGAMEOBJECTCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Core/Object/GameObject/GameObject.hpp"
#include "Engine/Vulkan/CommandPool/CommandPool.hpp"
#include "Engine/Vulkan/Descriptor/Layout/DescriptorSetLayout.hpp"
#include "Engine/Vulkan/Descriptor/Pool/DescriptorPool.hpp"
#include "Engine/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for VkGameObject Initialization
	*/
	struct VkGameObjectCreateInfo
	{
		/// physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		/// logical device
		VkDevice logicalDevice;

		/// graphics queue
		VkQueue graphicsQueue;

		/// command pool
		CommandPool* commandPool;

		GraphicsPipeline* graphicsPipeline;

		DescriptorSetLayout* descriptorSetLayout;

		DescriptorPool* descriptorPool;

		RenderEngine::Core::GameObject* gameObject = nullptr;

		/// default constructor
		VkGameObjectCreateInfo() = default;
	};
}

#endif