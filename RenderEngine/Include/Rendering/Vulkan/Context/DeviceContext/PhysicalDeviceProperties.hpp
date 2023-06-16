#pragma once

#ifndef RENDERENGINE_PHYSICALDEVICEPROPERTIES
#define RENDERENGINE_PHYSICALDEVICEPROPERTIES

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Context/DeviceContext/QueueFamilyIndices.hpp"


namespace RenderEngine::Rendering
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

		VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

		/// default constructor
		PhysicalDeviceProperties() = default;
		
		PhysicalDeviceProperties(VkPhysicalDevice _physicalDevice);
	};
}

#endif