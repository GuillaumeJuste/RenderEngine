#pragma once

#ifndef RENDERENGINE_SWAPCHAINCREATEINFO
#define RENDERENGINE_SWAPCHAINCREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Window/Surface.hpp"
#include "Engine/RenderPass/RenderPass.hpp"

namespace RenderEngine
{
	class Window;

	struct SwapChainCreateInfo
	{
		VkPhysicalDevice* physicalDevice = VK_NULL_HANDLE;
		VkDevice* logicalDevice = VK_NULL_HANDLE;
		Surface* surface = nullptr;
		Window* window = nullptr;
		RenderPass* renderPass = nullptr;

		SwapChainCreateInfo() = default;
	};
}

#endif