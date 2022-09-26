#pragma once

#ifndef RENDERENGINE_VKSCENECREATEINFO
#define RENDERENGINE_VKSCENECREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/CommandPool/CommandPool.hpp"
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

		RenderEngine::Core::Scene* scene;

		/// default constructor
		VkSceneCreateInfo() = default;
	};
}

#endif