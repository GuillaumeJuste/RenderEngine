#include "Core/RessourceManager/Texture.hpp"

using namespace RenderEngine::Core;

bool Texture::operator==(const Texture& _rhs) const
{
	return filePath == _rhs.filePath;
}