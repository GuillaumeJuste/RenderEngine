#pragma once

#ifndef RENDERENGINE_PHYSICALDEVICEPROPERTIES
#define RENDERENGINE_PHYSICALDEVICEPROPERTIES

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/DeviceContext/QueueFamilyIndices.hpp"


namespace RenderEngine::Engine::Vulkan
{
	/**
	 * @brief struct holding informations about a physical device
	*/
	struct PhysicalDeviceProperties
	{
		/**
		 * @brief Physical device
		*/
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		
		/**
		 * @brief Indice of the graphic and present queues  
		*/
		QueueFamilyIndices queueFamilyIndices;

		/**
		 * @brief properties of the physical device
		*/
		VkPhysicalDeviceProperties properties;

		/// default constructor
		PhysicalDeviceProperties() = default;
		
		PhysicalDeviceProperties(VkPhysicalDevice _physicalDevice);
	};
}

#endif