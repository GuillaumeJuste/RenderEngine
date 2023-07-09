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

		VkImage image = VK_NULL_HANDLE;
		VkImageView imageView = VK_NULL_HANDLE;
		VkDeviceMemory imageMemory = VK_NULL_HANDLE;
		VkImageLayout currentLayout;
		
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
		void TransitionImageLayout(VkImageLayout _newLayout, VkCommandBuffer _commandBuffer = nullptr);
		void CopyBufferToImage(VkBuffer _buffer, uint32_t _elementSize, bool _copyMipmap = false);
		bool CopyImageToBuffer(VkBuffer _buffer, uint32_t _elementSize);
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