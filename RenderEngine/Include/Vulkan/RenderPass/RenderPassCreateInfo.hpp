#pragma once

#ifndef RENDERENGINE_RENDERPASSCREATEINFO
#define RENDERENGINE_RENDERPASSCREATEINFO

#include "Vulkan/Shader/Shader.hpp"

namespace RenderEngine::Vulkan
{
	struct RenderPassCreateInfo
	{
		/// logical device
		VkDevice logicalDevice;

		/// swapchain image format
		VkFormat swapChainImageFormat;

		RenderPassCreateInfo() = default;
	};
}

#endif