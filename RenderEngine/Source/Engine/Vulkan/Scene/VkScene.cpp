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

	std::forward_list<GameObject> _gameObjects = _createInfo.scene->GetGameObjects();

	for (std::forward_list<GameObject>::iterator it = _gameObjects.begin(); it != _gameObjects.end(); ++it)
	{
		gaoCreateInfo.gameObject = &(*it);
		gameObjects.push_front(VkGameObject(gaoCreateInfo));
	}
}

void VkScene::Cleanup()
{
	for (std::forward_list<VkGameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		(*it).Cleanup();
	}
}