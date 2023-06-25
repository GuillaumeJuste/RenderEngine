#pragma once

#ifndef RENDERENGINE_IMAGE
#define RENDERENGINE_IMAGE

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Image/ImageVkCreateInfo.hpp"

namespace RenderEngine::Rendering
{
	/**
	 * @brief Class to manage creation and usage of Vulkan Image
	*/
	class Image
	{
	private:
		ImageVkCreateInfo createInfo;

		VkImage image;
		VkImageView imageView;
		VkDeviceMemory imageMemory;
		
		bool HasStencilComponent(VkFormat _format);

		void CreateImageView();
	public:

		/// default constructor
		Image() = default;

		/// default destructor
		~Image() = default;

		/**
		 * @brief Initiliaze Image
		 * @param _imageViewCreateInfo image view create info
		 * @param _output Image to initialize
		*/
		static void InitializeImage(const ImageVkCreateInfo& _imageCreateInfo, Image* _output);
		void TransitionImageLayout(VkFormat _format, VkImageLayout _oldLayout, VkImageLayout _newLayout, VkCommandBuffer _commandBuffer = nullptr);
		void CopyBufferToImage(VkBuffer _buffer);
		void GenerateMipmaps(VkFormat _format);

		/**
		 * @brief clean up vulkan classes
		*/
		void Cleanup();

		const VkImage& GetVkImage() const;
		const VkDeviceMemory& GetVkImageMemory() const;
		const VkImageView& Image::GetImageView() const;
	};

}

#endif