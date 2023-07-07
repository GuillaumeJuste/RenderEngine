#include "SceneGraph/Scene/Scene.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "SceneGraph/Components/MeshRenderer/MeshRenderer.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include <chrono>

using namespace RenderEngine::SceneGraph;

Scene::Scene(RenderEngine::ResourceManager* _resourceManager) :
	resourceManager{ _resourceManager }
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

	skybox.mesh = resourceManager->LoadMesh("Resources/Engine/Models/cube.obj");
	skybox.BRDFlut = resourceManager->LoadTexture("Resources/Engine/Textures/default_brdf_lut.png");
	skybox.vertexShader = resourceManager->LoadShader("Resources/Engine/Shaders/Skybox.vert.spv", VERTEX);
	skybox.fragmentShader = resourceManager->LoadShader("Resources/Engine/Shaders/Skybox.frag.spv", FRAGMENT);

	CubemapImportInfos importInfos("Resources/Engine/Textures/Skybox/right.jpg",
		"Resources/Engine/Textures/Skybox/left.jpg",
		"Resources/Engine/Textures/Skybox/top.jpg",
		"Resources/Engine/Textures/Skybox/bottom.jpg",
		"Resources/Engine/Textures/Skybox/front.jpg",
		"Resources/Engine/Textures/Skybox/back.jpg");

	skybox.cubemap = resourceManager->LoadCubemap(importInfos, false);
	skybox.irradianceMap = skybox.cubemap;
	skybox.prefilterMap = skybox.cubemap;
}

Scene::~Scene()
{
	timer.FixedUpdateEvent.Remove(this, &Scene::FixedUpdate);
}

void Scene::Initialize()
{
	mainCamera.Initialize();
	for (std::list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->Initialize();
	}
}

void Scene::Start()
{
	timer.Start();
	timer.FixedUpdateEvent.Add(this, &Scene::FixedUpdate);

	mainCamera.Start();
	for (std::list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->Start();
	}
}

void Scene::Update()
{
	timer.Update();

	mainCamera.Update(timer.GetDeltaTime());

	for (std::list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if(it->enable)
			it->Update(timer.GetDeltaTime());
	}
}

void Scene::FixedUpdate(double _deltaTime)
{
	mainCamera.FixedUpdate(timer.GetDeltaTime());
	for (std::list<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if (it->enable)
			it->FixedUpdate(_deltaTime);
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

	Mesh* mesh = resourceManager->LoadMesh("Resources/Engine/Models/cube.obj");
	Texture* texture = resourceManager->LoadTexture("Resources/Engine/Textures/White.jpg");
	MeshRenderer* meshRenderer = gao->AddComponent<MeshRenderer>();
	meshRenderer->mesh = mesh;
	meshRenderer->material.texture = texture;
	meshRenderer->material.metalnessMap = texture;
	meshRenderer->material.roughnessMap = texture;
	meshRenderer->material.normalMap = texture;
	meshRenderer->material.ambientOcclusionMap = texture;
	meshRenderer->vertexShader = resourceManager->LoadShader("Resources/Engine/Shaders/VertexShader.vert.spv", VERTEX);
	meshRenderer->fragmentShader = resourceManager->LoadShader("Resources/Engine/Shaders/BlinnPhongFragmentShader.frag.spv", FRAGMENT);

	mainCamera.SetParent(&rootObject);

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