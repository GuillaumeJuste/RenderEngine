#pragma once

#ifndef RENDERENGINE_VKTEXTURE
#define RENDERENGINE_VKTEXTURE

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Texture/VkTextureCreateInfo.hpp"
#include "Rendering/Vulkan/ImageView/ImageView.hpp"
#include "Rendering/Vulkan/Image/Image.hpp"

namespace RenderEngine::Rendering
{
	class VkTexture
	{
	private:

		VkTextureVkCreateInfo createInfo;

		Image image;
		ImageView imageView;
		VkSampler sampler;

		void CreateImageView(VkImageViewType _imageViewType, uint32_t _layerCount);
		void CreateSampler();
		void CopyBufferToImage(VkBuffer _buffer, uint32_t _width, uint32_t _height);

	public:
		VkTexture() = default;
		~VkTexture() = default;

		static void InitializeVkTexture(const VkTextureVkCreateInfo& _vkTextureCreateInfo, VkTexture* _output);

		/**
		 * @brief clean up vulkan classes
		*/
		void Cleanup();

		VkImageView GetImageView() const;
		VkSampler GetSampler() const;

	};

}

#endif