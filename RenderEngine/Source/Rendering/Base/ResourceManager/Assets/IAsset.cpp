#include "Rendering/Base/ResourceManager/Assets/IAsset.hpp"

using namespace RenderEngine::Assets;

bool IAsset::operator==(const IAsset& _rhs) const
{
	return filePath == _rhs.filePath;
}