#include "Engine/Vulkan/Scene/VkScene.hpp"
#include "Engine/Vulkan/Scene/VkGameObjectCreateInfo.hpp"

using namespace RenderEngine::Engine::Vulkan;

VkScene::VkScene(const VkSceneCreateInfo& _createInfo) :
	createInfo{ _createInfo }
{
	VkGameObjectCreateInfo gaoCreateInfo{};
	gaoCreateInfo.physicalDevice = _createInfo.physicalDevice;
	gaoCreateInfo.logicalDevice = _createInfo.logicalDevice;
	gaoCreateInfo.graphicsQueue = _createInfo.graphicsQueue;
	gaoCreateInfo.commandPool = _createInfo.commandPool;
	gaoCreateInfo.graphicsPipeline = createInfo.graphicsPipeline;

	CreateVkGameObjects(gaoCreateInfo, createInfo.scene->GetSceneRoot().GetChildrens());
}

void VkScene::CreateVkGameObjects(VkGameObjectCreateInfo _createInfo, std::vector<GameObject*> _childrens)
{
	for (std::vector<GameObject*>::iterator it = _childrens.begin(); it != _childrens.end(); ++it)
	{
		_createInfo.gameObject = (*it);
		gameObjects.push_front(VkGameObject(_createInfo));

		CreateVkGameObjects(_createInfo, (*it)->GetChildrens());
	}
}

std::forward_list<VkGameObject> VkScene::GetSceneObjects() const
{
	return gameObjects;
}

void VkScene::Update(size_t _currentframe)
{
	for (std::forward_list<VkGameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		it->Update(_currentframe);
	}
}

void VkScene::Cleanup()
{
	for (std::forward_list<VkGameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		(*it).Cleanup();
	}
}