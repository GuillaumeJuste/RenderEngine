#pragma once

#ifndef RENDERENGINE_DEVICECONTEXTVKCREATEINFO
#define RENDERENGINE_DEVICECONTEXTVkCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/WindowProperties/WindowProperties.hpp"

namespace RenderEngine::Engine::Vulkan
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