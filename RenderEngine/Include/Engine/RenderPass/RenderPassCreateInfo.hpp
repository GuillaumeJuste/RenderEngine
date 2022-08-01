#pragma once

#ifndef RENDERENGINE_RENDERPASSCREATEINFO
#define RENDERENGINE_RENDERPASSCREATEINFO

#include "Engine/Shader/Shader.hpp"

namespace RenderEngine::Vulkan
{
	struct RenderPassCreateInfo
	{
		VkDevice logicalDevice;
		VkFormat swapChainImageFormat;

		RenderPassCreateInfo() = default;
	};
}

#endif