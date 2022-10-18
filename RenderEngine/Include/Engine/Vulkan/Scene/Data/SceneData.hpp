#pragma once

#ifndef RENDERENGINE_SCENEDATA
#define RENDERENGINE_SCENEDATA

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Core/Scene/Scene.hpp"
#include "Engine/Vulkan/Scene/VkScene.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct SceneData
	{
		RenderEngine::Core::Scene* scene;

		VkScene vkScene;

		/// default constructor
		SceneData() = default;

		bool operator==(RenderEngine::Core::Scene* _scene);
	};
}

#endif