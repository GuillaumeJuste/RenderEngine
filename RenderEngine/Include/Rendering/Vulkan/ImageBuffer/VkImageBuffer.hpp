#pragma once

#ifndef RENDERENGINE_VKIMAGEBUFFER
#define RENDERENGINE_VKIMAGEBUFFER

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/ImageBuffer/VkImageBufferCreateInfo.hpp"

namespace RenderEngine::Rendering
{
	/**
	 * @brief Class to manage creation and usage of Vulkan Image
	*/
	class VkImageBuffer
	{
	private:
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice = VK_NULL_HANDLE;
		uint32_t width = 0u;
		uint32_t height = 0u;
		uint32_t textureCount = 0u;
		uint32_t mipLevels = 0u;

		VkFormat format = VK_FORMAT_UNDEFINED;
		VkQueue graphicsQueue = VK_NULL_HANDLE;
		CommandPool* commandPool = nullptr;

		VkImage image = VK_NULL_HANDLE;
		VkImageView imageView = VK_NULL_HANDLE;
		VkDeviceMemory imageMemory = VK_NULL_HANDLE;
		VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		
		bool HasStencilComponent(VkFormat _format);

		void CreateImageView(VkImageViewType _imageViewType, VkImageAspectFlags _imageViewAspectFlags);

		void FillDataFromCreateInfo(const VkImageBufferCreateInfo& _imageCreateInfo);
	public:

		/// default constructor
		VkImageBuffer() = default;

		/// default destructor
		~VkImageBuffer() = default;

		/**
		 * @brief Initiliaze Image
		 * @param _imageViewCreateInfo image view create info
		 * @param _output Image to initialize
		*/
		static void InitializeImageBuffer(const VkImageBufferCreateInfo& _imageCreateInfo, VkImageBuffer* _output);
		void TransitionImageLayout(VkImageLayout _newLayout, VkCommandBuffer _commandBuffer = nullptr);
		void CopyBufferToImage(VkBuffer _buffer, uint32_t _elementSize, VkCommandBuffer _commandBuffer = nullptr, bool _copyMipmap = false);
		bool CopyImageToBuffer(VkBuffer _buffer, uint32_t _elementSize);
		void GenerateMipmaps();

		/**
		 * @brief clean up vulkan classes
		*/
		void Cleanup();

		const VkImage& GetVkImage() const;
		const VkDeviceMemory& GetVkImageMemory() const;
		const VkImageView& GetImageView() const;
	};

}

#endif