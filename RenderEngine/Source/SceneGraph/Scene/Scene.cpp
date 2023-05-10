#include "SceneGraph/Scene/Scene.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "SceneGraph/Components/MeshRenderer/MeshRenderer.hpp"

using namespace RenderEngine::SceneGraph;

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

	skybox.mesh = ResourceManager::GetInstance()->LoadMesh("Resources/Engine/Models/cube.obj");
	skybox.front = ResourceManager::GetInstance()->LoadTexture("Resources/Engine/Textures/Skybox/front.jpg");
	skybox.back = ResourceManager::GetInstance()->LoadTexture("Resources/Engine/Textures/Skybox/back.jpg");
	skybox.left = ResourceManager::GetInstance()->LoadTexture("Resources/Engine/Textures/Skybox/left.jpg");
	skybox.right = ResourceManager::GetInstance()->LoadTexture("Resources/Engine/Textures/Skybox/right.jpg");
	skybox.top = ResourceManager::GetInstance()->LoadTexture("Resources/Engine/Textures/Skybox/top.jpg");
	skybox.bottom = ResourceManager::GetInstance()->LoadTexture("Resources/Engine/Textures/Skybox/bottom.jpg");

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

	Mesh* mesh = ResourceManager::GetInstance()->LoadMesh("Resources/Engine/Models/cube.obj");
	Texture* texture = ResourceManager::GetInstance()->LoadTexture("Resources/Engine/Textures/White.jpg");
	MeshRenderer* meshRenderer = gao->AddComponent<MeshRenderer>();
	meshRenderer->mesh = mesh;
	meshRenderer->texture = texture;
	meshRenderer->metalnessMap = texture;
	meshRenderer->roughnessMap = texture;
	meshRenderer->ambientOcclusionMap = texture;
	meshRenderer->vertexShaderFilePath = "Resources/Engine/Shaders/VertexShader.vert.spv";
	meshRenderer->fragmentShaderFilePath = "Resources/Engine/Shaders/BlinnPhongFragmentShader.frag.spv";

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