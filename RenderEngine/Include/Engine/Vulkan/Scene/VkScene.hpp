#pragma once

#ifndef RENDERENGINE_VKSCENE
#define RENDERENGINE_VKSCENE

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Scene/VkSceneCreateInfo.hpp"
#include "Engine/Vulkan/Scene/VkGameObjectCreateInfo.hpp"
#include "Engine/Vulkan/Scene/VkGameObject.hpp"

#include <forward_list>
#include <vector>

namespace RenderEngine::Engine::Vulkan
{
	class VkScene
	{
	private:
		VkSceneCreateInfo createInfo;

		std::forward_list<VkGameObject> gameObjects;

		void CreateVkGameObjects(VkGameObjectCreateInfo _createInfo, std::vector<GameObject*> _childrens);

	public:
		VkScene() = default;
		VkScene(const VkSceneCreateInfo& _createInfo);
		~VkScene() = default;

		void Cleanup();

		std::forward_list<VkGameObject> GetSceneObjects() const;
	};

}

#endif