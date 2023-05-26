#include "Rendering/Vulkan/Scene/VkScene.hpp"
#include "Rendering/Vulkan/Scene/GameObject/VkGameObjectCreateInfo.hpp"
#include "Rendering/Vulkan/UniformBuffer/UniformBufferData.hpp"
#include "SceneGraph/Scene/Scene.hpp"
#include "SceneGraph/Object/GameObject/Camera/Camera.hpp"

using namespace RenderEngine::Rendering;
using namespace RenderEngine::SceneGraph;

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

	CreateLightBuffer(minimumLightCount, minimumLightCount, minimumLightCount);

	CreateSkybox();

	gaoCreateInfo.cameraBuffer = &cameraBuffer;
	gaoCreateInfo.pointLightsBuffer = &pointLightsBuffer;
	gaoCreateInfo.directionalLightsBuffer = &directionalLightsBuffer;
	gaoCreateInfo.spotLightsBuffer = &spotLightsBuffer;
	gaoCreateInfo.irradianceMap = skybox.GetIrradianceCubeMap();
	gaoCreateInfo.prefilterMap = skybox.GetPrefilterCubemap();
	gaoCreateInfo.BRDFlut = skybox.GetBRDFlut();

	CreateVkGameObjects(gaoCreateInfo, createInfo.scene->GetSceneRoot().GetChildrens());

	CreateLightBuffer(scenePointLights.size(), sceneDirectionalLights.size(), sceneSpotLights.size());
}

void VkScene::CreateVkGameObjects(VkGameObjectCreateInfo _createInfo, std::vector<GameObject*> _childrens)
{
	for (std::vector<GameObject*>::iterator it = _childrens.begin(); it != _childrens.end(); ++it)
	{
		_createInfo.gameObject = (*it);

		VkGameObject* gao = &gameObjects.emplace_front(VkGameObject(_createInfo));

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
		data.intensity = it->pointlight->intensity;

		lightsdata.push_back(data);
	}

	for (size_t i = lightsdata.size(); i < minimumLightCount; i++)
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
		data.intensity = it->directionalLight->intensity;

		lightsdata.push_back(data);
	}

	for (size_t i = lightsdata.size(); i < minimumLightCount; i++)
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
		data.intensity = it->spotLight->intensity;
		data.range = it->spotLight->range;
		data.cutOff = Mathlib::Math::Cos(it->spotLight->cutOff * Mathlib::Math::DegToRad);

		lightsdata.push_back(data);
	}

	for (size_t i = lightsdata.size(); i < minimumLightCount; i++)
	{
		SpotLightData data{};
		data.enable = false;
		lightsdata.push_back(data);
	}
	return lightsdata;
}

void VkScene::CreateLightBuffer(size_t _pointLightCount, size_t _directionalLightCount, size_t _spotLightCount)
{
	size_t pointLightBufferSize = sizeof(PointLightData) * _pointLightCount;
	if (pointLightBufferSize > pointLightsBuffer.GetBufferSize())
	{
		BufferObjectVkCreateInfo pointLightsBufferCreateInfo;
		pointLightsBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
		pointLightsBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
		pointLightsBufferCreateInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		pointLightsBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		pointLightsBufferCreateInfo.bufferSize = pointLightBufferSize;

		DescriptorBuffer::InitializeDescriptorBuffer(pointLightsBufferCreateInfo, MAX_FRAMES_IN_FLIGHT, &pointLightsBuffer);
	}

	size_t directionalLightBufferSize = sizeof(DirectionalLightData) * _directionalLightCount;
	if (directionalLightBufferSize > directionalLightsBuffer.GetBufferSize())
	{
	BufferObjectVkCreateInfo directionalLightsBufferCreateInfo;
	directionalLightsBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	directionalLightsBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	directionalLightsBufferCreateInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	directionalLightsBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	directionalLightsBufferCreateInfo.bufferSize = directionalLightBufferSize;

	DescriptorBuffer::InitializeDescriptorBuffer(directionalLightsBufferCreateInfo, MAX_FRAMES_IN_FLIGHT, &directionalLightsBuffer);
	}

	size_t spotLightBufferSize = sizeof(SpotLightData) * _spotLightCount;
	if (spotLightBufferSize > spotLightsBuffer.GetBufferSize())
	{
		BufferObjectVkCreateInfo spotLightsBufferCreateInfo;
		spotLightsBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
		spotLightsBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
		spotLightsBufferCreateInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		spotLightsBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		spotLightsBufferCreateInfo.bufferSize = spotLightBufferSize;

		DescriptorBuffer::InitializeDescriptorBuffer(spotLightsBufferCreateInfo, MAX_FRAMES_IN_FLIGHT, &spotLightsBuffer);
	}
}

void VkScene::CreateSkybox()
{
	skybox.skybox = &createInfo.scene->skybox;

	VkSkyboxCreateInfo skyboxCreateInfo{};
	skyboxCreateInfo.physicalDevice = createInfo.physicalDevice;
	skyboxCreateInfo.logicalDevice = createInfo.logicalDevice;
	skyboxCreateInfo.renderpass = createInfo.renderpass;
	skyboxCreateInfo.swapchain = createInfo.swapchain;

	skybox.InitializeSkybox(skyboxCreateInfo, &cameraBuffer);
}


void VkScene::Draw(VkCommandBuffer _commandBuffer, int _currentFrame) 
{
	for (std::forward_list<VkGameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->Draw(_commandBuffer, _currentFrame);
	}
	skybox.Draw(_commandBuffer, _currentFrame);
}

void VkScene::Update(size_t _currentframe)
{
	Camera* camera = createInfo.scene->GetCamera();
	VkExtent2D extent = createInfo.swapchain->GetExtent();

	CameraBufferData cameraBufferdata{};
	cameraBufferdata.invView = camera->GetInvViewMatrix().Transpose();
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

	skybox.Clean();

	for (std::forward_list<VkGameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		(*it).Cleanup();
	}
}