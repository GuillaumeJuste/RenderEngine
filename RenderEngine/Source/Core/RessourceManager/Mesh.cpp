#include "Core/RessourceManager/Mesh.hpp"

using namespace RenderEngine::Core;

bool Mesh::operator==(const Mesh& _rhs) const
{
	return name == _rhs.name && filePath == _rhs.filePath;
}