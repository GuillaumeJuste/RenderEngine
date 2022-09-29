#pragma once

#ifndef RENDERENGINE_VKSCENE
#define RENDERENGINE_VKSCENE

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Scene/VkSceneCreateInfo.hpp"
#include "Engine/Vulkan/Scene/VkGameObject.hpp"

namespace RenderEngine::Engine::Vulkan
{
	class VkScene
	{
	private:
		VkSceneCreateInfo createInfo;

		std::forward_list<VkGameObject> gameObjects;

	public:
		VkScene() = default;
		VkScene(const VkSceneCreateInfo& _createInfo);
		~VkScene() = default;

		void Cleanup();

		std::forward_list<VkGameObject> GetSceneObjects() const;
	};

}

#endif