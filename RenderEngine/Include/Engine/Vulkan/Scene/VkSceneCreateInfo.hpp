#pragma once

#ifndef RENDERENGINE_VKSCENECREATEINFO
#define RENDERENGINE_VKSCENECREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/CommandPool/CommandPool.hpp"
#include "Engine/Vulkan/Descriptor/Layout/DescriptorSetLayout.hpp"
#include "Engine/Vulkan/Descriptor/Pool/DescriptorPool.hpp"
#include "Engine/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Core/Scene/Scene.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for VkGameObject Initialization
	*/
	struct VkSceneCreateInfo
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

		RenderEngine::Core::Scene* scene;

		/// default constructor
		VkSceneCreateInfo() = default;
	};
}

#endif