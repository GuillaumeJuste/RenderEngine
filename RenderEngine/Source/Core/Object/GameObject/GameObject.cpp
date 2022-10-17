#include "Core/Object/GameObject/GameObject.hpp"
#include "Core/Components/Material/Material.hpp"
#include "Core/Components/MeshRenderer/MeshRenderer.hpp"
#include "Core/RessourceManager/RessourceManager.hpp"

using namespace RenderEngine::Core;

void GameObject::InitializeGameObject(const GameObjectCreateInfo& _createinfo, GameObject* _output)
{
	_output->parent = _createinfo.parent;
	_output->transform = Transform(_createinfo.transform);
	_output->name = _createinfo.name;

	Mesh* mesh = RessourceManager::GetInstance()->LoadMesh("Resources/Models/cube.obj", "Cube");
	MeshRenderer* meshRenderer = _output->AddComponent<MeshRenderer>();
	meshRenderer->SetMesh(mesh);

	Texture* texture = RessourceManager::GetInstance()->LoadTexture("Resources/Textures/White.jpg");
	Material* material = _output->AddComponent<Material>();
	material->SetTexture(texture); 
}

void GameObject::Initialize()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->Initialize();
	}
}

void GameObject::Start()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->Start();
	}
}

void GameObject::Update()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if((*it)->enable)
			(*it)->Update();
	}
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

Mathlib::Transform GameObject::GetLocalTransform()
{
	return transform.GetLocalTransform();
}

void GameObject::SetLocalTransform(Mathlib::Transform _newTransform)
{
	transform.SetLocalTransform(_newTransform);
}

std::vector<GameObject*> GameObject::GetChildrens() const
{
	return childrens;
}

void GameObject::SetParent(GameObject* _newParent)
{
	if(_newParent != this)
		parent = _newParent;
}

void GameObject::Cleanup()
{
	size_t size = components.size();
	for (size_t i = 0; i < size; i++)
	{
		delete components[i];
	}
	components.clear();
}