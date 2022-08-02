#include "Core/Scene/Scene.hpp"

using namespace RenderEngine::Core;

void Scene::InitializeScene(const SceneCreateInfo& _createinfo, Scene* _output)
{
	_output->name = _createinfo.name;
	_output->rootObject.SetIsHidden(true);
	_output->rootObject.SetName("root");
}

GameObject* Scene::AddGameObject(GameObjectCreateInfo _createInfo)
{
	GameObject gao;
	GameObject::InitializeGameObject(_createInfo, &gao);

	gameObjects.push_back(gao);

	_createInfo.parent->AddChild(&gao);

	return &gameObjects[gameObjects.size()-1];
}

bool Scene::RemoveGameObject(GameObject* _gao)
{
	size_t size = gameObjects.size();

	for (size_t i = 0; i < size; i++)
	{
		if (gameObjects[i].GetUId() == _gao->GetUId())
		{
			size_t childrenNumber = gameObjects[i].GetChildrens().size();

			if (childrenNumber > 0)
			{
				std::vector<GameObject*> childrensList = gameObjects[i].GetChildrens();

				for (size_t j = 0; j < size; j++)
				{
					childrensList[j]->SetParent(&rootObject);
				}
			}

			gameObjects.erase(gameObjects.begin() + i);
			return true;
		}
	}
	return false;
}

const GameObject* Scene::GetGameObjectByName(std::string _name) const
{
	size_t size = gameObjects.size();

	for (size_t i = 0; i < size; i++)
	{
		if (gameObjects[i].GetName() == _name)
		{
			return &gameObjects[i];
		}
	}

	return nullptr;
}

const GameObject* Scene::GetGameObjectByID(unsigned int _id) const
{
	size_t size = gameObjects.size();

	for (size_t i = 0; i < size; i++)
	{
		if (gameObjects[i].GetUId() == _id)
		{
			return &gameObjects[i];
		}
	}

	return nullptr;
}

GameObject* Scene::GetSceneRoot()
{
	return &rootObject;
}