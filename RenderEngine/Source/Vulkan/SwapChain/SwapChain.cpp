#include "Vulkan/SwapChain/SwapChain.hpp"

#include <cstdint>
#include <limits> 
#include <stdexcept>
#include <iostream>

#include "Vulkan/ImageView/ImageViewCreateInfo.hpp"
#include "Misc/Math.hpp"

using namespace RenderEngine::Vulkan;
using namespace Mathlib;

void SwapChain::InitializeSwapChain(const SwapChainCreateInfo& _swapChainCreateInfo, SwapChain* _output)
{
	_output->surface = _swapChainCreateInfo.surface;
	_output->window = _swapChainCreateInfo.window;
	_output->logicalDevice = _swapChainCreateInfo.logicalDevice;
	_output->queueFamilyIndices = _swapChainCreateInfo.queueFamilyIndices;
	_output->CreateVkSwapChain(_swapChainCreateInfo);
	_output->CreateImageView();
}

void SwapChain::CreateVkSwapChain(const SwapChainCreateInfo& _swapChainCreateInfo)
{
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(_swapChainCreateInfo.physicalDevice, *surface);

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface->GetVkSurface();

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices =  queueFamilyIndices;
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) 
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else 
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &vkSwapChain) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(logicalDevice, vkSwapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(logicalDevice, vkSwapChain, &imageCount, swapChainImages.data());

	swapChainImageCount = imageCount;
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

void SwapChain::CreateImageView()
{
	ImageViewCreateInfo createInfo;
	createInfo.logicalDevice = logicalDevice;
	createInfo.swapChainImageCount = swapChainImageCount;
	createInfo.swapChainImages = swapChainImages;
	createInfo.swapChainImageFormat = swapChainImageFormat;

	ImageView::InitializeImageView(createInfo, &imageView);
}

SwapChainSupportDetails SwapChain::QuerySwapChainSupport(const VkPhysicalDevice& _device, const Surface& _surface)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device, _surface.GetVkSurface(), &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(_device, _surface.GetVkSurface(), &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(_device, _surface.GetVkSurface(), &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(_device, _surface.GetVkSurface(), &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(_device, _surface.GetVkSurface(), &presentModeCount, details.presentModes.data());
	}
	return details;
}

VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _availableFormats)
{
	for (const auto& availableFormat : _availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return _availableFormats[0];
}

VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _availablePresentModes)
{
	for (const auto& availablePresentMode : _availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& _capabilities)
{
	if (_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return _capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(window->GetGLFWWindow(), &width, &height);

		width = Math::Clamp(width, _capabilities.minImageExtent.width, _capabilities.maxImageExtent.width);
		height = Math::Clamp(height, _capabilities.minImageExtent.height, _capabilities.maxImageExtent.height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		return actualExtent;
	}
}

void SwapChain::Cleanup()
{
	imageView.Cleanup();
	vkDestroySwapchainKHR(logicalDevice, vkSwapChain, nullptr);
	std::cout << "[Cleaned] Swap Chain" << std::endl;
}

const VkSwapchainKHR& SwapChain::GetVKSwapChain() const
{
	return vkSwapChain;
}

const VkExtent2D& SwapChain::GetSwapChainExtent() const
{
	return swapChainExtent;
}

const VkFormat& SwapChain::GetSwapChainImageFormat() const
{
	return swapChainImageFormat;
}

const ImageView& SwapChain::GetImageView() const
{
	return imageView;
}

const size_t& SwapChain::GetImageImageCount() const
{
	return swapChainImageCount;
}