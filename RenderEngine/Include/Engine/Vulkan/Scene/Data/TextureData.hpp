#pragma once

#ifndef RENDERENGINE_TEXTUREDATA
#define RENDERENGINE_TEXTUREDATA

#include "Core/RessourceManager/Texture.hpp"
#include "Engine/Vulkan/Texture/VkTexture.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct TextureData
	{
		RenderEngine::Core::Texture* texture;

		VkTexture vkTexture;

		/// default constructor
		TextureData() = default;

		bool operator==(RenderEngine::Core::Texture* _texture) const;

		void Cleanup();
	};
}

#endif