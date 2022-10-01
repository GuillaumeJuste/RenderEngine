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
	gaoCreateInfo.descriptorSetLayout = createInfo.descriptorSetLayout;
	gaoCreateInfo.graphicsPipeline = createInfo.graphicsPipeline;
	gaoCreateInfo.descriptorPool = createInfo.descriptorPool;

	std::vector<GameObject*> _gameObjects = _createInfo.scene->GetGameObjects();

	for (std::vector<GameObject*>::iterator it = _gameObjects.begin(); it != _gameObjects.end(); ++it)
	{
		gaoCreateInfo.gameObject = (*it);
		gameObjects.push_front(VkGameObject(gaoCreateInfo));
	}
}


std::forward_list<VkGameObject> VkScene::GetSceneObjects() const
{
	return gameObjects;
}

void VkScene::Cleanup()
{
	for (std::forward_list<VkGameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		(*it).Cleanup();
	}
}