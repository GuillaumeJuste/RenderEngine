#pragma once

#ifndef RENDERENGINE_TEXTUREDATA
#define RENDERENGINE_TEXTUREDATA

#include "ResourceManager/Assets/Texture/Texture.hpp"
#include "Engine/Vulkan/Texture/VkTexture.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct TextureData
	{
		RenderEngine::Assets::Texture* texture = nullptr;

		VkTexture vkTexture;

		/// default constructor
		TextureData() = default;

		bool operator==(RenderEngine::Assets::Texture* _texture) const;

		void Cleanup();
	};
}

#endif