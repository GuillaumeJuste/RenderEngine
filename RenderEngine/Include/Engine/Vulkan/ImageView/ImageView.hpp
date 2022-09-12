#pragma once

#ifndef RENDERENGINE_IMAGEVIEW
#define RENDERENGINE_IMAGEVIEW

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/ImageView/ImageViewVkCreateInfo.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	 * @brief Class to manage creation and usage of Vulkan ImageView
	*/
	class ImageView
	{
	private:
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;
		
		/// Image views
		std::vector<VkImageView> swapChainImageViews;

	public:

		/// default constructor
		ImageView() = default;

		/// default destructor
		~ImageView() = default;

		/**
		 * @brief Initiliaze ImageView
		 * @param _imageViewCreateInfo image view create info 
		 * @param _output ImageView to initialize
		*/
		static void InitializeImageView(ImageViewVkCreateInfo _imageViewCreateInfo, ImageView* _output);
		
		/**
		 * @brief clean up vulkan classes
		*/
		void Cleanup();

		const std::vector<VkImageView>& GetSwapChainImageViews() const;
	};

}

#endif