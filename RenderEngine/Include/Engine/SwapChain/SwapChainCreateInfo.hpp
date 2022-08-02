#pragma once

#ifndef RENDERENGINE_SWAPCHAINCREATEINFO
#define RENDERENGINE_SWAPCHAINCREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Surface/Surface.hpp"
#include "Engine/RenderPass/RenderPass.hpp"
#include "Engine/DeviceContext/QueueFamiliyIndices.hpp"
#include "Engine/Window/Window.hpp"

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