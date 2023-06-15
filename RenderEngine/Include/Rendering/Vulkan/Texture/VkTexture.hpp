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
		VkSampler sampler;

		void CreateSampler();

	public:
		VkTexture() = default;
		~VkTexture() = default;

		static void InitializeVkTexture(const VkTextureVkCreateInfo& _vkTextureCreateInfo, VkTexture* _output);

		/**
		 * @brief clean up vulkan classes
		*/
		void Clean();

		VkImageView GetImageView() const;
		VkSampler GetSampler() const;

	};

}

#endif