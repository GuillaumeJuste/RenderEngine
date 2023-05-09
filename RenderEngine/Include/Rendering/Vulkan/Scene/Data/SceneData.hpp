#pragma once

#ifndef RENDERENGINE_SCENEDATA
#define RENDERENGINE_SCENEDATA

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "SceneGraph/Scene/Scene.hpp"
#include "Rendering/Vulkan/Scene/VkScene.hpp"

namespace RenderEngine::Rendering
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