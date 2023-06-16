#pragma once

#ifndef RENDERENGINE_VKSCENECREATEINFO
#define RENDERENGINE_VKSCENECREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/CommandPool/CommandPool.hpp"
#include "Rendering/Vulkan/Descriptor/Layout/DescriptorSetLayout.hpp"
#include "Rendering/Vulkan/Descriptor/Pool/DescriptorPool.hpp"
#include "Rendering/Vulkan/SwapChain/SwapChain.hpp"
#include "Rendering/Vulkan/RenderPass/RenderPass.hpp"
#include "SceneGraph/Scene/Scene.hpp"

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for VkGameObject Initialization
	*/
	struct VkSceneCreateInfo
	{
		/// physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;

		/// logical device
		VkDevice logicalDevice;

		/// graphics queue
		VkQueue graphicsQueue;

		/// command pool
		CommandPool* commandPool;

		SwapChain* swapchain;

		RenderPass* renderpass;

		RenderEngine::SceneGraph::Scene* scene;

		/// default constructor
		VkSceneCreateInfo() = default;
	};
}

#endif