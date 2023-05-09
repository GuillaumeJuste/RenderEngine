#pragma once

#ifndef RENDERENGINE_IMAGEVIEW
#define RENDERENGINE_IMAGEVIEW

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/ImageView/ImageViewVkCreateInfo.hpp"

namespace RenderEngine::Rendering
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
		VkImageView imageView;

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

		const VkImageView& GetImageView() const;
	};

}

#endif