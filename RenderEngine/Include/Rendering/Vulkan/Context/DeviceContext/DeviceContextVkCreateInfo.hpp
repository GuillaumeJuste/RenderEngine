#pragma once

#ifndef RENDERENGINE_DEVICECONTEXTVKCREATEINFO
#define RENDERENGINE_DEVICECONTEXTVkCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/WindowProperties/WindowProperties.hpp"

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for DeviceContext Initialization
	*/
	struct DeviceContextVkCreateInfo
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