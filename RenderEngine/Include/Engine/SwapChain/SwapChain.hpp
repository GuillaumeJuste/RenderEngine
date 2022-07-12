#pragma once

#ifndef RENDERENGINE_SWAPCHAIN
#define RENDERENGINE_SWAPCHAIN

#include "Vulkan/VulkanBaseInclude.hpp"

#include "Engine/Window/Surface.hpp"
#include "Engine/ImageView/ImageView.hpp"

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
		VkDevice logicalDevice = VK_NULL_HANDLE;

		VkSwapchainKHR vkSwapChain = VK_NULL_HANDLE;
		size_t swapChainImageCount;
		std::vector<VkImage> swapChainImages;
		
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;

		ImageView imageView;

		void CreateVkSwapChain(const SwapChainCreateInfo& _swapChainCreateInfo);
		void CreateImageView();

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& _capabilities);

	public:
		SwapChain() = default;
		~SwapChain() = default;

		static void InitializeSwapChain(const SwapChainCreateInfo& _swapChainCreateInfo, SwapChain* _output);
		void Cleanup();

		const VkSwapchainKHR& GetVKSwapChain() const;
		
		static SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& _device, const Surface& _surface);
	};

}

#endif