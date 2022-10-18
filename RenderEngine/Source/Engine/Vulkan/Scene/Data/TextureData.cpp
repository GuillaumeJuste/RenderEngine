#include "Engine/Vulkan/Scene/Data/TextureData.hpp"

using namespace RenderEngine::Engine::Vulkan;

bool TextureData::operator==(RenderEngine::Core::Texture* _texture) const
{
	return texture == _texture;
}