#pragma once

#ifndef RENDERENGINE_VKGAMEOBJECTCREATEINFO
#define RENDERENGINE_VKGAMEOBJECTCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "SceneGraph/Object/GameObject/GameObject.hpp"
#include "Rendering/Vulkan/SwapChain/SwapChain.hpp"
#include "Rendering/Vulkan/RenderPass/RenderPass.hpp"
#include "Rendering/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"
#include "Rendering/Vulkan/Texture/VkTexture.hpp"

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for VkGameObject Initialization
	*/
	struct VkGameObjectCreateInfo
	{
		/// physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;

		/// logical device
		VkDevice logicalDevice;

		SwapChain* swapchain;

		RenderPass* renderpass;

		RenderEngine::SceneGraph::GameObject* gameObject = nullptr;

		DescriptorBuffer* cameraBuffer;
		DescriptorBuffer* pointLightsBuffer;
		DescriptorBuffer* directionalLightsBuffer;
		DescriptorBuffer* spotLightsBuffer;
		VkTexture* skyboxMap;
		VkTexture* irradianceMap;
		VkTexture* prefilterMap;
		VkTexture* BRDFlut;

		/// default constructor
		VkGameObjectCreateInfo() = default;
	};
}

#endif