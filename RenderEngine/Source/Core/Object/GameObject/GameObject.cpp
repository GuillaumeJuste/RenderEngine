#include "Core/Object/GameObject/GameObject.hpp"

using namespace RenderEngine::Core;
using namespace Mathlib;

void GameObject::InitializeGameObject(const GameObjectCreateInfo& _createinfo, GameObject* _output)
{
	_output->mesh = _createinfo.mesh;
	_output->parent = _createinfo.parent;
	_output->transform = _createinfo.transform;
	_output->name = _createinfo.name;
}

void GameObject::AddChild(GameObject* _newChild)
{
	childrens.push_back(_newChild);
}

bool GameObject::RemoveChild(GameObject* _child)
{
	size_t size = childrens.size();

	for (size_t i = 0; i < size; i++)
	{
		if (childrens[i]->GetUId() == _child->GetUId())
		{
			childrens.erase(childrens.begin() + i);
			return true;
		}
	}
	return false;
}

Mesh* GameObject::GetMesh() const
{
	return mesh;
}

GameObject* GameObject::GetParent() const
{
	return parent;
}

const Transform& GameObject::GetTransform() const
{
	return transform;
}

Transform GameObject::GetWorldTransform()
{
	if (parent == nullptr)
	{
		return transform;
	}

	Transform parentTransform = parent->GetWorldTransform();

	return Transform::GetWorldTransfrom(parentTransform, transform);
}

std::vector<GameObject*> GameObject::GetChildrens() const
{
	return childrens;
}

void GameObject::SetParent(GameObject* _newParent)
{
	parent = _newParent;
}
