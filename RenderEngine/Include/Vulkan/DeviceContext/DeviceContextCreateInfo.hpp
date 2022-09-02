#pragma once

#ifndef RENDERENGINE_DEVICECONTEXTCREATEINFO
#define RENDERENGINE_DEVICECONTEXTCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/WindowProperties/WindowProperties.hpp"

namespace RenderEngine::Vulkan
{
	/**
	* @brief struct holding data for DeviceContext Initialization
	*/
	struct DeviceContextCreateInfo
	{
		/**
		 * @brief VkInstance initializing the logical device
		*/
		VkInstance instance = VK_NULL_HANDLE;

		/**
		 * @brief window used for render
		*/
		WindowProperties* windowProperties;
	};
}

#endif