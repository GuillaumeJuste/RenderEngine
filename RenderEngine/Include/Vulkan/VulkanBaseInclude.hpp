#pragma once

#ifndef RENDERENGINE_VULKANBASEINCLUDE
#define RENDERENGINE_VULKANBASEINCLUDE

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vector>

namespace RenderEngine
{
	const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
}

#endif