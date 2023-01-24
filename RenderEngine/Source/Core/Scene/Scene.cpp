#include "Core/Scene/Scene.hpp"
#include "Core/RessourceManager/RessourceManager.hpp"
#include "Core/Components/MeshRenderer/MeshRenderer.hpp"

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

	GameObjectCreateInfo mainCameraCreateInfo{};
	mainCameraCreateInfo.name = "mainCamera";
	mainCameraCreateInfo.parent = &rootObject;

	GameObject::InitializeGameObject(mainCameraCreateInfo, &mainCamera);
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
	mainCamera.Update();

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

	Mesh* mesh = RessourceManager::GetInstance()->LoadMesh("Resources/Models/cube.obj");
	Texture* texture = RessourceManager::GetInstance()->LoadTexture("Resources/Textures/White.jpg");
	MeshRenderer* meshRenderer = gao->AddComponent<MeshRenderer>();
	meshRenderer->mesh = mesh;
	meshRenderer->texture = texture;
	meshRenderer->metalnessMap = texture;
	meshRenderer->roughnessMap = texture;
	meshRenderer->ambientOcclusionMap = texture;
	meshRenderer->vertexShaderFilePath = "Resources/Shaders/VertexShader.spv";
	meshRenderer->fragmentShaderFilePath = "Resources/Shaders/BlinnPhongFragmentShader.spv";

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

Camera* Scene::GetCamera()
{
	return &mainCamera;
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