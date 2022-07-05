#pragma once

#ifndef RENDERENGINE_SWAPCHAIN
#define RENDERENGINE_SWAPCHAIN

#include "Vulkan/VulkanBaseInclude.hpp"

#include "Engine/Surface.hpp"

namespace RenderEngine
{
	struct SwapChainSupportDetails;
	struct SwapChainCreateInfo;
	class Window;

	class SwapChain
	{
	private:
		Surface surface;
		Window* window;
		VkDevice logicalDevice;

		VkSwapchainKHR vkSwapChain;
		std::vector<VkImage> swapChainImages;
		
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& _capabilities);

	public:
		SwapChain() = default;
		~SwapChain() = default;

		void InitializeSwapChain(const SwapChainCreateInfo& _swapChainCreateInfo);
		void Cleanup();

		const VkSwapchainKHR& GetVKSwapChain() const;
		
		static SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& _device, const Surface& _surface);
	};

}

#endif