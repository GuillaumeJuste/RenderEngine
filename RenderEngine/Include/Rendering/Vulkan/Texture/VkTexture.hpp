#pragma once

#ifndef RENDERENGINE_VKTEXTURE
#define RENDERENGINE_VKTEXTURE

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Texture/VkTextureCreateInfo.hpp"
#include "Rendering/Vulkan/Image/Image.hpp"
#include "Rendering/Base/Interface/Primitive/ITexture.hpp"

namespace RenderEngine::Rendering
{
	class VkTexture : public ITexture
	{
	private:

		VkTextureVkCreateInfo createInfo;

		Image image;
		VkSampler sampler = VK_NULL_HANDLE;

		void CreateSampler(uint32_t _mipmap);

		template<typename T>
		bool FillImageBuffer(T* _imageData);

	public:
		VkTexture() = default;
		~VkTexture() = default;

		static void InitializeVkTexture(const VkTextureVkCreateInfo& _vkTextureCreateInfo, VkTexture* _output, bool _fillImage = true);

		/**
		 * @brief clean up vulkan classes
		*/
		void Clean();

		Image* GetImage();
		VkImageView GetImageView() const;
		VkSampler GetSampler() const;

	};

}

#include "Rendering/Vulkan/Texture/VkTexture.inl"

#endif