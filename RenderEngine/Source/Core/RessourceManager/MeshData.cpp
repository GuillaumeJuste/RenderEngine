#include "Core/RessourceManager/MeshData.hpp"

using namespace RenderEngine::Core;

bool MeshData::operator==(const MeshData& _rhs) const
{
	return mesh.name == _rhs.mesh.name;
}