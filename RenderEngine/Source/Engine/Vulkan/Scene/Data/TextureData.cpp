#include "Engine/Vulkan/Scene/Data/TextureData.hpp"

using namespace RenderEngine::Engine::Vulkan;

bool TextureData::operator==(RenderEngine::Assets::Texture* _texture) const
{
	return texture == _texture;
}

void TextureData::Cleanup()
{
	vkTexture.Cleanup();
}