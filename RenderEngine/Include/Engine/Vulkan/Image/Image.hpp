#pragma once

#ifndef RENDERENGINE_IMAGE
#define RENDERENGINE_IMAGE

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Image/ImageVkCreateInfo.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	 * @brief Class to manage creation and usage of Vulkan ImageView
	*/
	class Image
	{
	private:
		ImageVkCreateInfo createInfo;

		VkImage image;
		VkDeviceMemory imageMemory;
		
		bool HasStencilComponent(VkFormat _format);

	public:

		/// default constructor
		Image() = default;

		/// default destructor
		~Image() = default;

		/**
		 * @brief Initiliaze ImageView
		 * @param _imageViewCreateInfo image view create info
		 * @param _output ImageView to initialize
		*/
		static void InitializeImage(ImageVkCreateInfo _imageCreateInfo, Image* _output);
		void TransitionImageLayout(VkFormat _format, VkImageLayout _oldLayout, VkImageLayout _newLayout);

		/**
		 * @brief clean up vulkan classes
		*/
		void Cleanup();

		const VkImage& GetVkImage() const;
		const VkDeviceMemory& GetVkImageMemory() const;
	};

}

#endif