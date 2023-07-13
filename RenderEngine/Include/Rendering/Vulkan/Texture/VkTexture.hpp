#pragma once

#ifndef RENDERENGINE_VKTEXTURE
#define RENDERENGINE_VKTEXTURE

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Texture/VkTextureCreateInfo.hpp"
#include "Rendering/Vulkan/ImageBuffer/VkImageBuffer.hpp"
#include "Rendering/Base/Interface/Primitive/ITexture.hpp"

namespace RenderEngine::Rendering
{
	class VkTexture : public ITexture
	{
	private:
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;

		/// physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		/// graphics queue
		VkQueue graphicsQueue = VK_NULL_HANDLE;

		/// command pool
		CommandPool* commandPool = nullptr;

		uint32_t elementSize = 0;

		VkImageBuffer imageBuffer;
		VkSampler sampler = VK_NULL_HANDLE;

		void CreateSampler(uint32_t _mipmap);

		void FillDataFromCreateInfo(const VkTextureVkCreateInfo& _vkTextureCreateInfo);

	public:
		VkTexture() = default;
		~VkTexture() = default;

		static void InitializeVkTexture(const VkTextureVkCreateInfo& _vkTextureCreateInfo, VkTexture* _output);

		template<typename T>
		bool FillImageBuffer(T* _imageData, uint32_t _totalImageSize, bool _copyMipMap);

		/**
		 * @brief clean up vulkan classes
		*/
		void Clean();

		VkImageBuffer* GetImageBuffer();
		VkImageView GetImageView() const;
		VkSampler GetSampler() const;

		void GetTextureData(char* _output, uint32_t _imageSize);

	};

}

#include "Rendering/Vulkan/Texture/VkTexture.inl"

#endif