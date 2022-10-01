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

	rootObject.SetIsEnabled(false);
}

void Scene::Initialize()
{
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		(*it)->Initialize();
	}
}

void Scene::Start()
{
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		(*it)->Start();
	}
}

void Scene::Update()
{
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		(*it)->Update();
	}
}

GameObject* Scene::AddGameObject(GameObjectCreateInfo _createInfo)
{
	GameObject* gao = new GameObject();

	if (_createInfo.parent != nullptr)
	{
		_createInfo.parent->AddChild(gao);
	}	
	else
	{
		_createInfo.parent = &rootObject;
	}	

	GameObject::InitializeGameObject(_createInfo, gao);

	gameObjects.push_back(gao);

	return gao;
}

bool Scene::RemoveGameObject(GameObject* _gao)
{
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if ((*it)->GetUId() == _gao->GetUId())
		{
			size_t childrenNumber = (*it)->GetChildrens().size();

			if (childrenNumber > 0)
			{
				std::vector<GameObject*> childrensList = (*it)->GetChildrens();

				size_t size = childrensList.size();
				for (size_t j = 0; j < size; j++)
				{
					childrensList[j]->SetParent(_gao->GetParent());
				}
			}
			gameObjects.erase(it);
			break;
		}
	}

	return false;
}

const GameObject* Scene::GetGameObjectByName(std::string _name)
{
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if ((*it)->GetName() == _name)
		{
			return (*it);
		}
	}

	return nullptr;
}

const GameObject* Scene::GetGameObjectByID(unsigned int _id)
{
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if ((*it)->GetUId() == _id)
		{
			return (*it);
		}
	}

	return nullptr;
}

GameObject* Scene::GetSceneRoot()
{
	return &rootObject;
}

const std::vector<GameObject*>& Scene::GetGameObjects() const
{
	return gameObjects;
}

void Scene::Cleanup()
{
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		(*it)->Cleanup();

		delete (*it);
	}
}

bool Scene::operator==(const Scene& _scene) const
{
	return name == _scene.name;
}