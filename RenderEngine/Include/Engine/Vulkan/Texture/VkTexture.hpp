#pragma once

#ifndef RENDERENGINE_VKTEXTURE
#define RENDERENGINE_VKTEXTURE

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Texture/VkTextureCreateInfo.hpp"

namespace RenderEngine::Engine::Vulkan
{
	class VkTexture
	{
	private:

		VkTextureVkCreateInfo createInfo;

		VkImage image;
		VkDeviceMemory imageMemory;
		VkImageView imageView;
		VkSampler sampler;

		void CreateImage();
		void CreateImageView();
		void CreateSampler();
		uint32_t FindMemoryType(uint32_t _typeFilter, VkMemoryPropertyFlags _properties);
		void TransitionImageLayout(VkFormat _format, VkImageLayout _oldLayout, VkImageLayout _newLayout);
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