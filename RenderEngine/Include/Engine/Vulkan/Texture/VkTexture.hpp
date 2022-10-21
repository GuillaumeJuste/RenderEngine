#pragma once

#ifndef RENDERENGINE_VKTEXTURE
#define RENDERENGINE_VKTEXTURE

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Texture/VkTextureCreateInfo.hpp"
#include "Engine/Vulkan/ImageView/ImageView.hpp"
#include "Engine/Vulkan/Image/Image.hpp"

namespace RenderEngine::Engine::Vulkan
{
	class VkTexture
	{
	private:

		VkTextureVkCreateInfo createInfo;

		Image image;
		ImageView imageView;
		VkSampler sampler;

		void CreateImageView();
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