#pragma once

#ifndef RENDERENGINE_SWAPCHAIN
#define RENDERENGINE_SWAPCHAIN

#include "Vulkan/Misc/VulkanBaseInclude.hpp"

#include "Vulkan/Surface/Surface.hpp"
#include "Vulkan/ImageView/ImageView.hpp"
#include "Vulkan/SwapChain/SwapChainCreateInfo.hpp"
#include "Vulkan/SwapChain/SwapChainSupportDetails.hpp"
#include "Glfw/Window/Window.hpp"

namespace RenderEngine::Vulkan
{
	/**
	 * @brief Class to manage creation and usage of Vulkan SwapChain
	*/
	class SwapChain
	{
	private:
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;
		
		/// window properties
		WindowProperties* windowProperties;

		/// Queue family indices
		QueueFamilyIndices queueFamilyIndices;

		/// Swapchain
		VkSwapchainKHR vkSwapChain = VK_NULL_HANDLE;
		
		/// swapchain Images
		std::vector<VkImage> images;
		
		/// swapchain image format
		VkFormat imageFormat;

		/// swapchain extent
		VkExtent2D extent;

		/// Image view
		ImageView imageView;

		/**
		 * @brief Create Swapchain
		 * @param _swapChainCreateInfo swap chain create info
		*/
		void CreateVkSwapChain(const SwapChainCreateInfo& _swapChainCreateInfo);
		
		/**
		 * @brief Create ImageView
		*/
		void CreateImageView();

		/**
		 * @brief Choose swapchain image format from formats available
		 * @param _availableFormats Available image formats
		 * @return swapchain image format
		*/
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _availableFormats);
		
		/**
		 * @brief Choose swapchain present mode from available present mode
		 * @param _availablePresentModes Available present modes
		 * @return swapchain present mode
		*/
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _availablePresentModes);

		/**
		 * @brief Choose swapchain extent
		 * @param _capabilities surface capabilites
		 * @return swapchain extent
		*/
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& _capabilities);

	public:
		/// default constructor
		SwapChain() = default;

		/// default destructor
		~SwapChain() = default;

		/**
		 * @brief Initialize swapchain
		 * @param _swapChainCreateInfo  swapchain createinfo
		 * @param _output swapchain to initilize
		*/
		static void InitializeSwapChain(const SwapChainCreateInfo& _swapChainCreateInfo, SwapChain* _output);
		
		/**
		 * @brief Cleanup vulkan classes
		*/
		void Cleanup();

		/// Get swapchain
		const VkSwapchainKHR& GetVKSwapChain() const;
		
		/// Get swapchain extent
		const VkExtent2D& GetExtent() const;
		
		/// Get swapchain image format
		const VkFormat& GetImageFormat() const;
		
		/// Get swapchain imageview
		const ImageView& GetImageView() const;
		
		/// Get swapchain image count
		const size_t& GetImageCount() const;

		/**
		 * @brief Query Swapchain support
		 * @param _device physical device
		 * @param _surface surface
		 * @return swapchain support detail
		*/
		static SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& _device, const Surface& _surface);
	};

}

#endif