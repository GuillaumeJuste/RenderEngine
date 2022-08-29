#pragma once

#ifndef RENDERENGINE_IMAGEVIEW
#define RENDERENGINE_IMAGEVIEW

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/ImageView/ImageViewCreateInfo.hpp"

namespace RenderEngine::Vulkan
{
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
		static void InitializeImageView(ImageViewCreateInfo _imageViewCreateInfo, ImageView* _output);
		
		/**
		 * @brief clean up vulkan classes
		*/
		void Cleanup();

		const std::vector<VkImageView>& GetSwapChainImageViews() const;
	};

}

#endif