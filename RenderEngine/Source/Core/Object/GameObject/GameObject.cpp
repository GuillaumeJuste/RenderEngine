#include "Core/Object/GameObject/GameObject.hpp"

using namespace RenderEngine::Core;

GameObject::~GameObject()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		delete *it;
	}
	components.clear();
}

void GameObject::InitializeGameObject(const GameObjectCreateInfo& _createinfo, GameObject* _output)
{
	_output->parent = _createinfo.parent;
	_output->transform = Transform(_createinfo.transform);
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

GameObject* GameObject::GetParent() const
{
	return parent;
}


Mathlib::Transform GameObject::GetWorldTransform()
{
	if (parent == nullptr)
	{
		return transform.GetLocalTransform();
	}

	Mathlib::Transform parentTransform = parent->GetWorldTransform();

	return Mathlib::Transform::GetWorldTransfrom(parentTransform, transform.GetLocalTransform());
}

std::vector<GameObject*> GameObject::GetChildrens() const
{
	return childrens;
}

void GameObject::SetParent(GameObject* _newParent)
{
	parent = _newParent;
}
