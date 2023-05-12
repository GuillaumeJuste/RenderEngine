#pragma once

#ifndef RENDERENGINE_VKGAMEOBJECTCREATEINFO
#define RENDERENGINE_VKGAMEOBJECTCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "SceneGraph/Object/GameObject/GameObject.hpp"
#include "Rendering/Vulkan/SwapChain/SwapChain.hpp"
#include "Rendering/Vulkan/RenderPass/RenderPass.hpp"
#include "Rendering/Vulkan/Scene/Data/MeshData.hpp"
#include "Rendering/Vulkan/Scene/Data/TextureData.hpp"

namespace RenderEngine::Rendering
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

		TextureData* textureData;
		TextureData* metalnessMap;
		TextureData* roughnessMap;
		TextureData* aoMap;

		RenderEngine::SceneGraph::GameObject* gameObject = nullptr;

		/// default constructor
		VkGameObjectCreateInfo() = default;
	};
}

#endif