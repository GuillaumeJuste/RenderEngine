#pragma once

#ifndef RENDERENGINE_SWAPCHAINCREATEINFO
#define RENDERENGINE_SWAPCHAINCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/Surface/Surface.hpp"
#include "Vulkan/RenderPass/RenderPass.hpp"
#include "Vulkan/DeviceContext/QueueFamilyIndices.hpp"
#include "Glfw/Window/Window.hpp"

namespace RenderEngine::Vulkan
{
	struct SwapChainCreateInfo
	{
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice = VK_NULL_HANDLE;
		Surface* surface = nullptr;
		Window* window = nullptr;
		RenderPass* renderPass = nullptr;
		QueueFamilyIndices queueFamilyIndices;

		SwapChainCreateInfo() = default;
	};
}

#endif