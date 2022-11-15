#include "Engine/Vulkan/Scene/VkScene.hpp"
#include "Engine/Vulkan/Scene/GameObject/VkGameObjectCreateInfo.hpp"
#include "Engine/Vulkan/UniformBuffer/UniformBufferData.hpp"
#include "Core/Scene/Scene.hpp"
#include "Core/Object/GameObject/Camera/Camera.hpp"

using namespace RenderEngine::Engine::Vulkan;
using namespace RenderEngine::Core;

VkScene::VkScene(const VkSceneCreateInfo& _createInfo) :
	createInfo{ _createInfo }
{
	VkGameObjectCreateInfo gaoCreateInfo{};
	gaoCreateInfo.physicalDevice = _createInfo.physicalDevice;
	gaoCreateInfo.logicalDevice = _createInfo.logicalDevice;
	gaoCreateInfo.renderpass = _createInfo.renderpass;
	gaoCreateInfo.swapchain = _createInfo.swapchain;

	BufferObjectVkCreateInfo cameraBufferCreateInfo;
	cameraBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	cameraBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	cameraBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	cameraBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	cameraBufferCreateInfo.bufferSize = sizeof(CameraBufferData);

	DescriptorBuffer::InitializeDescriptorBuffer(cameraBufferCreateInfo, MAX_FRAMES_IN_FLIGHT, &cameraBuffer);

	CreateVkGameObjects(gaoCreateInfo, createInfo.scene->GetSceneRoot().GetChildrens());

	CreateLightBuffer();

	for (std::forward_list<VkGameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->CreateDescriptorSet(&cameraBuffer, &pointLightsBuffer, &directionalLightsBuffer, &spotLightsBuffer);
	}
}

void VkScene::CreateVkGameObjects(VkGameObjectCreateInfo _createInfo, std::vector<GameObject*> _childrens)
{
	for (std::vector<GameObject*>::iterator it = _childrens.begin(); it != _childrens.end(); ++it)
	{
		_createInfo.gameObject = (*it);

		MeshRenderer* meshRenderer = _createInfo.gameObject->GetComponent<MeshRenderer>();
		if (meshRenderer != nullptr)
		{
			_createInfo.meshData = LoadMesh(meshRenderer->GetMesh());
			_createInfo.textureData = LoadTexture(meshRenderer->GetTexture());
			_createInfo.specularMap = LoadTexture(meshRenderer->GetSpecularMap());
		}

		gameObjects.push_front(VkGameObject(_createInfo));

		CreateVkGameObjects(_createInfo, (*it)->GetChildrens());

		PointLight* pointLight = _createInfo.gameObject->GetComponent<PointLight>();
		if (pointLight != nullptr)
		{
			VkPointLight vkLight{};
			vkLight.gameObject = (*it);
			vkLight.pointlight = pointLight;

			scenePointLights.push_back(vkLight);
		}

		SpotLight* spotLight = _createInfo.gameObject->GetComponent<SpotLight>();
		if (spotLight != nullptr)
		{
			VkSpotLight vkLight{};
			vkLight.gameObject = (*it);
			vkLight.spotLight = spotLight;

			sceneSpotLights.push_back(vkLight);
		}

		DirectionalLight*directionalLight = _createInfo.gameObject->GetComponent<DirectionalLight>();
		if (directionalLight != nullptr)
		{
			VkDirectionalLight vkLight{};
			vkLight.gameObject = (*it);
			vkLight.directionalLight = directionalLight;

			sceneDirectionalLights.push_back(vkLight);
		}
	}
}

std::vector<PointLightData> VkScene::GeneratePointLightsData()
{
	std::vector<PointLightData> lightsdata;
	for (std::vector<VkPointLight>::iterator it = scenePointLights.begin(); it != scenePointLights.end(); ++it)
	{
		PointLightData data{};
		data.enable = it->pointlight->enable;
		data.position = it->gameObject->GetWorldTransform().position;
		data.color = it->pointlight->color;
		data.range = it->pointlight->range;
		data.ambient = it->pointlight->ambient;
		data.diffuse = it->pointlight->diffuse;
		data.specular = it->pointlight->specular;

		lightsdata.push_back(data);
	}

	for (int i = lightsdata.size(); i < minimumLightCount; i++)
	{
		PointLightData data{};
		data.enable = false;
		lightsdata.push_back(data);
	}


	return lightsdata;
}

std::vector<DirectionalLightData> VkScene::GenerateDirectionalLightsData()
{
	std::vector<DirectionalLightData> lightsdata;
	for (std::vector<VkDirectionalLight>::iterator it = sceneDirectionalLights.begin(); it != sceneDirectionalLights.end(); ++it)
	{
		DirectionalLightData data{};
		data.enable = it->directionalLight->enable;
		data.color = it->directionalLight->color;
		data.direction = it->gameObject->GetWorldTransform().GetForwardVector();
		data.ambient = it->directionalLight->ambient;
		data.diffuse = it->directionalLight->diffuse;
		data.specular = it->directionalLight->specular;

		lightsdata.push_back(data);
	}

	for (int i = lightsdata.size(); i < minimumLightCount; i++)
	{
		DirectionalLightData data{};
		data.enable = false;
		lightsdata.push_back(data);
	}
	return lightsdata;
}

std::vector<SpotLightData> VkScene::GenerateSpotLightsData()
{
	std::vector<SpotLightData> lightsdata;
	for (std::vector<VkSpotLight>::iterator it = sceneSpotLights.begin(); it != sceneSpotLights.end(); ++it)
	{
		SpotLightData data{};
		data.enable = it->spotLight->enable;
		data.color = it->spotLight->color;
		data.position = it->gameObject->GetWorldTransform().position;
		data.direction = it->gameObject->GetWorldTransform().GetForwardVector();
		data.range = it->spotLight->range;
		data.cutOff = it->spotLight->cutOff;
		data.ambient = it->spotLight->ambient;
		data.diffuse = it->spotLight->diffuse;
		data.specular = it->spotLight->specular;

		lightsdata.push_back(data);
	}

	for (int i = lightsdata.size(); i < minimumLightCount; i++)
	{
		SpotLightData data{};
		data.enable = false;
		lightsdata.push_back(data);
	}
	return lightsdata;
}

void VkScene::CreateLightBuffer()
{
	std::vector<PointLightData> pointLightsdata = GeneratePointLightsData();

	int pointLightCount = pointLightsdata.size() > minimumLightCount ? pointLightsdata.size() : minimumLightCount;

	BufferObjectVkCreateInfo pointLightsBufferCreateInfo;
	pointLightsBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	pointLightsBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	pointLightsBufferCreateInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	pointLightsBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	pointLightsBufferCreateInfo.bufferSize = sizeof(PointLightData) * pointLightCount;

	DescriptorBuffer::InitializeDescriptorBuffer(pointLightsBufferCreateInfo, MAX_FRAMES_IN_FLIGHT, &pointLightsBuffer);


	std::vector<DirectionalLightData> directionalLightsdata = GenerateDirectionalLightsData();

	int directionalLightCount = directionalLightsdata.size() > minimumLightCount ? directionalLightsdata.size() : minimumLightCount;

	BufferObjectVkCreateInfo directionalLightsBufferCreateInfo;
	directionalLightsBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	directionalLightsBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	directionalLightsBufferCreateInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	directionalLightsBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	directionalLightsBufferCreateInfo.bufferSize = sizeof(DirectionalLightData) * directionalLightCount;

	DescriptorBuffer::InitializeDescriptorBuffer(directionalLightsBufferCreateInfo, MAX_FRAMES_IN_FLIGHT, &directionalLightsBuffer);

	std::vector<SpotLightData> spotLightsdata = GenerateSpotLightsData();

	int spotLightCount = spotLightsdata.size() > minimumLightCount ? spotLightsdata.size() : minimumLightCount;

	BufferObjectVkCreateInfo spotLightsBufferCreateInfo;
	spotLightsBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	spotLightsBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	spotLightsBufferCreateInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	spotLightsBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	spotLightsBufferCreateInfo.bufferSize = sizeof(SpotLightData) * spotLightCount;

	DescriptorBuffer::InitializeDescriptorBuffer(spotLightsBufferCreateInfo, MAX_FRAMES_IN_FLIGHT, &spotLightsBuffer);
}

MeshData* VkScene::LoadMesh(RenderEngine::Core::Mesh* _mesh)
{
	MeshData* meshData = GetMesh(_mesh);

	if (meshData != nullptr)
		return meshData;

	MeshData* newMesh = new MeshData();
	newMesh->mesh = _mesh;
	CreateVertexBufferObject(_mesh, newMesh);
	CreateIndexBufferObject(_mesh, newMesh);

	sceneMeshes.push_back(newMesh);
	return newMesh;
}

MeshData* VkScene::GetMesh(RenderEngine::Core::Mesh* _mesh)
{
	for (std::vector<MeshData*>::iterator it = sceneMeshes.begin(); it != sceneMeshes.end(); ++it)
	{
		if ((*it)->mesh == _mesh)
		{
			return (*it);
		}
	}
	return nullptr;
}

void VkScene::CreateVertexBufferObject(RenderEngine::Core::Mesh* _mesh, MeshData* _output)
{
	std::vector<Vertex> vertices = _mesh->vertices;

	BufferObject stagingBufferObject;
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	stagingBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = sizeof(vertices[0]) * vertices.size();

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBufferObject);

	void* data;
	vkMapMemory(createInfo.logicalDevice, stagingBufferObject.GetVkBufferMemory(), 0, stagingBufferCreateInfo.bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)stagingBufferCreateInfo.bufferSize);
	vkUnmapMemory(createInfo.logicalDevice, stagingBufferObject.GetVkBufferMemory());

	BufferObjectVkCreateInfo vertexBuffeCreateInfo;
	vertexBuffeCreateInfo.physicalDevice = createInfo.physicalDevice;
	vertexBuffeCreateInfo.logicalDevice = createInfo.logicalDevice;
	vertexBuffeCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	vertexBuffeCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	vertexBuffeCreateInfo.bufferSize = stagingBufferCreateInfo.bufferSize;

	BufferObject::InitializeBufferObject(vertexBuffeCreateInfo, &_output->vertexBufferObject);

	stagingBufferObject.CopyBuffer(&_output->vertexBufferObject, createInfo.commandPool, createInfo.graphicsQueue, stagingBufferCreateInfo.bufferSize);
	stagingBufferObject.Cleanup();
}

void VkScene::CreateIndexBufferObject(RenderEngine::Core::Mesh* _mesh, MeshData* _output)
{
	std::vector<uint16_t> indices = _mesh->indices;

	BufferObject stagingBufferObject;
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	stagingBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = sizeof(indices[0]) * indices.size();

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBufferObject);

	void* data;
	vkMapMemory(createInfo.logicalDevice, stagingBufferObject.GetVkBufferMemory(), 0, stagingBufferCreateInfo.bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)stagingBufferCreateInfo.bufferSize);
	vkUnmapMemory(createInfo.logicalDevice, stagingBufferObject.GetVkBufferMemory());

	BufferObjectVkCreateInfo indexBufferCreateInfo;
	indexBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	indexBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	indexBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	indexBufferCreateInfo.bufferSize = stagingBufferCreateInfo.bufferSize;

	BufferObject::InitializeBufferObject(indexBufferCreateInfo, &_output->indexBufferObject);

	stagingBufferObject.CopyBuffer(&_output->indexBufferObject, createInfo.commandPool, createInfo.graphicsQueue, stagingBufferCreateInfo.bufferSize);
	stagingBufferObject.Cleanup();
}


TextureData* VkScene::LoadTexture(RenderEngine::Core::Texture* _texture)
{
	TextureData* textureData = GetTexture(_texture);

	if (textureData != nullptr)
		return textureData;

	TextureData* newTexture = new TextureData();
	newTexture->texture = _texture;
	CreateVkTexture(_texture, newTexture);

	sceneTextures.push_back(newTexture);
	return newTexture;
}

TextureData* VkScene::GetTexture(RenderEngine::Core::Texture* _texture)
{
	for (std::vector<TextureData*>::iterator it = sceneTextures.begin(); it != sceneTextures.end(); ++it)
	{
		if (**it == _texture)
		{
			return (*it);
		}
	}

	return nullptr;
}

void VkScene::CreateVkTexture(RenderEngine::Core::Texture* _texture, TextureData* _output)
{
	VkTextureVkCreateInfo textCreateInfo{};
	textCreateInfo.logicalDevice = createInfo.logicalDevice;
	textCreateInfo.physicalDevice = createInfo.physicalDevice;
	textCreateInfo.graphicsQueue = createInfo.graphicsQueue;
	textCreateInfo.commandPool = createInfo.commandPool;
	textCreateInfo.texture = _texture;
	textCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	textCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	textCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	textCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	VkTexture::InitializeVkTexture(textCreateInfo, &_output->vkTexture);
}

void VkScene::Draw(VkCommandBuffer _commandBuffer, int _currentFrame) 
{
	for (std::forward_list<VkGameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->Draw(_commandBuffer, _currentFrame);
	}
}

void VkScene::Update(size_t _currentframe)
{
	Camera* camera = createInfo.scene->GetCamera();
	VkExtent2D extent = createInfo.swapchain->GetExtent();

	CameraBufferData cameraBufferdata{};
	cameraBufferdata.invView = camera->GetViewMatrix().Transpose();
	cameraBufferdata.proj = camera->GetProjectionMatrix((float)extent.width / (float)extent.height).Transpose();
	cameraBufferdata.cameraPos = camera->GetWorldTransform().position;
	cameraBuffer.CopyDataToBuffer<CameraBufferData>((int)_currentframe, &cameraBufferdata, sizeof(CameraBufferData));

	std::vector<PointLightData> pointLightsdata = GeneratePointLightsData();
	pointLightsBuffer.CopyDataToBuffer<PointLightData>((int)_currentframe, pointLightsdata.data(), sizeof(PointLightData) * pointLightsdata.size());

	std::vector<DirectionalLightData> directionalLightsdata = GenerateDirectionalLightsData();
	directionalLightsBuffer.CopyDataToBuffer<DirectionalLightData>((int)_currentframe, directionalLightsdata.data(), sizeof(DirectionalLightData) * directionalLightsdata.size());

	std::vector<SpotLightData> spotLightsdata = GenerateSpotLightsData();
	spotLightsBuffer.CopyDataToBuffer<SpotLightData>((int)_currentframe, spotLightsdata.data(), sizeof(SpotLightData) * spotLightsdata.size());

	for (std::forward_list<VkGameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->Update(_currentframe);
	}
}

void VkScene::Cleanup()
{
	cameraBuffer.Cleanup();
	pointLightsBuffer.Cleanup();
	directionalLightsBuffer.Cleanup();
	spotLightsBuffer.Cleanup();

	for (std::forward_list<VkGameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		(*it).Cleanup();
	}

	for (std::vector<MeshData*>::iterator it = sceneMeshes.begin(); it != sceneMeshes.end(); ++it)
	{
		(*it)->Cleanup();
		delete (*it);
	}

	for (std::vector<TextureData*>::iterator it = sceneTextures.begin(); it != sceneTextures.end(); ++it)
	{
		(*it)->Cleanup();
		delete (*it);
	}
}