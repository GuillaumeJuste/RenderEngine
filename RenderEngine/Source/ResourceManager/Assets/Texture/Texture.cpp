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

uint32_t Texture::ComputeTotalSize(int _width, int _height, int _channels, int _mipmapLevels)
{
	uint32_t totalSize = 0u;

	for (uint32_t i = 0u; i < _mipmapLevels; ++i)
	{
		totalSize += _width * _height;

		if (_width > 1)
			_width >>= 1;

		if (_height > 1)
			_height >>= 1;
	}

	return totalSize * _channels;
}