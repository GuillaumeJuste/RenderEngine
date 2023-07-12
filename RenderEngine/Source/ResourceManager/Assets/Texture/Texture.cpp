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

uint32_t Texture::ComputeTotalSize(Texture* _texture)
{
	uint32_t totalSize = 0u;

	int tmpWidth = _texture->width;
	int tmpHeight = _texture->height;

	for (uint32_t i = 0u; i < _texture->mipLevels; ++i)
	{
		totalSize += tmpWidth * tmpHeight;

		if (tmpWidth > 1)
			tmpWidth >>= 1;

		if (tmpHeight > 1)
			tmpHeight >>= 1;
	}

	totalSize *= _texture->channels;
	return totalSize * (_texture->isHDR ? sizeof(float) : 1.f);
}