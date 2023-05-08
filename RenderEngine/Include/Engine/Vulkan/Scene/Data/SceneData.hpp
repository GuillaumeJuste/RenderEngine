#pragma once

#ifndef RENDERENGINE_SCENEDATA
#define RENDERENGINE_SCENEDATA

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "SceneGraph/Scene/Scene.hpp"
#include "Engine/Vulkan/Scene/VkScene.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct SceneData
	{
		RenderEngine::SceneGraph::Scene* scene;

		VkScene vkScene;

		/// default constructor
		SceneData() = default;

		bool operator==(RenderEngine::SceneGraph::Scene* _scene);
	};
}

#endif