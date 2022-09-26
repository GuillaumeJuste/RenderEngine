#pragma once

#ifndef RENDERENGINE_VKGAMEOBJECTCREATEINFO
#define RENDERENGINE_VKGAMEOBJECTCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Core/Object/GameObject/GameObject.hpp"
#include "Engine/Vulkan/CommandPool/CommandPool.hpp"

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

		RenderEngine::Core::GameObject* gameObject = nullptr;

		/// default constructor
		VkGameObjectCreateInfo() = default;
	};
}

#endif