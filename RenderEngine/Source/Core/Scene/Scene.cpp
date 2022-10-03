#include "Core/Scene/Scene.hpp"

using namespace RenderEngine::Core;

Scene::Scene()
{
	GameObjectCreateInfo rootCreateInfo{};
	rootCreateInfo.name = "root";
	rootCreateInfo.parent = nullptr;
	
	Mathlib::Transform transform;
	transform.scale = Mathlib::Vec3(1.f, 1.f, 1.f);

	rootCreateInfo.transform = transform;

	GameObject::InitializeGameObject(rootCreateInfo, &rootObject);

	rootObject.enable = false;
}

void Scene::Initialize()
{
	for (std::list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->Initialize();
	}
}

void Scene::Start()
{
	for (std::list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->Start();
	}
}

void Scene::Update()
{
	for (std::list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if(it->enable)
			it->Update();
	}
}

GameObject* Scene::AddGameObject(GameObjectCreateInfo _createInfo)
{
	GameObject* gao = &gameObjects.emplace_front();

	if (_createInfo.parent != nullptr)
	{
		_createInfo.parent->AddChild(gao);
	}	
	else
	{
		_createInfo.parent = &rootObject;
		rootObject.AddChild(gao);
	}	

	GameObject::InitializeGameObject(_createInfo, gao);

	return gao;
}

bool Scene::RemoveGameObject(GameObject* _gao)
{
	for (std::list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if (it->GetUId() == _gao->GetUId())
		{
			std::vector<GameObject*> childrensList = it->GetChildrens();
			size_t size = childrensList.size();
			for (size_t j = 0; j < size; j++)
			{
				childrensList[j]->SetParent(_gao->GetParent());
			}
			gameObjects.erase(it);
			return true;
		}
	}

	return false;
}

const GameObject* Scene::GetGameObjectByName(std::string _name)
{
	for (std::list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if (it->name == _name)
		{
			return &(*it);
		}
	}

	return nullptr;
}

const GameObject* Scene::GetGameObjectByID(unsigned int _id)
{
	for (std::list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if (it->GetUId() == _id)
		{
			return &(*it);
		}
	}

	return nullptr;
}

const GameObject& Scene::GetSceneRoot()
{
	return rootObject;
}

void Scene::Cleanup()
{
	for (std::list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->Cleanup();
	}
}

bool Scene::operator==(const Scene& _scene) const
{
	return name == _scene.name;
}