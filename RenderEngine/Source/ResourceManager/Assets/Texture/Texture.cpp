#include "ResourceManager/Assets/Texture/Texture.hpp"

using namespace RenderEngine::Assets;

Texture::~Texture()
{
	if (iTexture != nullptr)
	{
		iTexture->Clean();
		delete iTexture;
	}
}