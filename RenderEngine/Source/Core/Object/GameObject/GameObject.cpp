#include "Core/Object/GameObject/GameObject.hpp"

using namespace RenderEngine::Core;
using namespace Mathlib;

void GameObject::InitializeGameObject(const GameObjectCreateInfo& _createinfo, GameObject* _output)
{
	_output->mesh = _createinfo.mesh;
	_output->parent = _createinfo.parent;
	_output->transform = _createinfo.transform;
}

void GameObject::AddChild(Object* _newChild)
{
	childrens.push_back(_newChild);
}

bool GameObject::RemoveChild(Object* _child)
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

Mesh* GameObject::GetMesh()
{
	return mesh;
}

Object* GameObject::GetParent()
{
	return parent;
}

const Mathlib::Transform& GameObject::GetTransform() const
{
	return transform;
}

std::vector<Object*> GameObject::GetChildrens()
{
	return childrens;
}
