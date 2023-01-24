#pragma once

#ifndef RENDERENGINE_VKGAMEOBJECTCREATEINFO
#define RENDERENGINE_VKGAMEOBJECTCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Core/Object/GameObject/GameObject.hpp"
#include "Engine/Vulkan/CommandPool/CommandPool.hpp"
#include "Engine/Vulkan/Descriptor/Layout/DescriptorSetLayout.hpp"
#include "Engine/Vulkan/Descriptor/Pool/DescriptorPool.hpp"
#include "Engine/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Engine/Vulkan/SwapChain/SwapChain.hpp"
#include "Engine/Vulkan/RenderPass/RenderPass.hpp"
#include "Engine/Vulkan/Scene/Data/MeshData.hpp"
#include "Engine/Vulkan/Scene/Data/TextureData.hpp"

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

		SwapChain* swapchain;

		RenderPass* renderpass;

		MeshData* meshData;
		TextureData* textureData;
		TextureData* metalnessMap;
		TextureData* roughnessMap;
		TextureData* aoMap;

		RenderEngine::Core::GameObject* gameObject = nullptr;

		/// default constructor
		VkGameObjectCreateInfo() = default;
	};
}

#endif