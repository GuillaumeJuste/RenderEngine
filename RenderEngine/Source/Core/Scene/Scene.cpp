#include "Core/Scene/Scene.hpp"

using namespace RenderEngine::Core;

Scene::Scene()
{
	rootObject.SetIsEnabled(false);
	rootObject.SetName("root");
}

void Scene::Initialize()
{
	for (std::forward_list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->Initialize();
	}
}

void Scene::Start()
{
	for (std::forward_list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->Start();
	}
}

void Scene::Update()
{
	for (std::forward_list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
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
	}	

	GameObject::InitializeGameObject(_createInfo, gao);

	return gao;
}

bool Scene::RemoveGameObject(GameObject* _gao)
{
	bool gaoFound = false;
	for (std::forward_list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if (it->GetUId() == _gao->GetUId())
		{
			size_t childrenNumber = it->GetChildrens().size();

			if (childrenNumber > 0)
			{
				std::vector<GameObject*> childrensList = it->GetChildrens();

				size_t size = childrensList.size();
				for (size_t j = 0; j < size; j++)
				{
					childrensList[j]->SetParent(_gao->GetParent());
				}
			}
			gaoFound = true;
			break;

		}
	}
	if (gaoFound)
	{
		gameObjects.remove(*_gao);
		return true;
	}

	return false;
}

const GameObject* Scene::GetGameObjectByName(std::string _name)
{
	for (std::forward_list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if (it->GetName() == _name)
		{
			return &(* it);
		}
	}

	return nullptr;
}

const GameObject* Scene::GetGameObjectByID(unsigned int _id)
{
	for (std::forward_list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if (it->GetUId() == _id)
		{
			return &(*it);
		}
	}

	return nullptr;
}

GameObject* Scene::GetSceneRoot()
{
	return &rootObject;
}

std::forward_list<GameObject> Scene::GetGameObjects() const
{
	return gameObjects;
}

void Scene::Cleanup()
{
	for (std::forward_list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		(*it).Cleanup();
	}
}

bool Scene::operator==(const Scene& _scene) const
{
	return name == _scene.name;
}