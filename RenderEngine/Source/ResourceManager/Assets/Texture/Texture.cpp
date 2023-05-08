#include "ResourceManager/Assets/Texture/Texture.hpp"

using namespace RenderEngine::Assets;

bool Texture::operator==(const Texture& _rhs) const
{
	return filePath == _rhs.filePath;
}