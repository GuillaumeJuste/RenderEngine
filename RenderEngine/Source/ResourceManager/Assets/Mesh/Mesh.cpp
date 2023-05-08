#include "ResourceManager/Assets/Mesh/Mesh.hpp"

using namespace RenderEngine::Assets;

bool Mesh::operator==(const Mesh& _rhs) const
{
	return name == _rhs.name && filePath == _rhs.filePath;
}