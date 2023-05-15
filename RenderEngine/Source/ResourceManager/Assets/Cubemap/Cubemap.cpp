#include "ResourceManager/Assets/Cubemap/Cubemap.hpp"

using namespace RenderEngine::Assets;

Cubemap::~Cubemap()
{
	if (iTexture != nullptr)
	{
		iTexture->Clean();
		delete iTexture;
	}
}