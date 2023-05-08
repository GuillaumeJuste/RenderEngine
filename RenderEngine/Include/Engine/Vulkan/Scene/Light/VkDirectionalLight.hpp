#pragma once

#ifndef RENDERENGINE_VKDIRECTIONALLIGHT
#define RENDERENGINE_VKDIRECTIONALLIGHT

#include "SceneGraph/Components/Light/DirectionalLight.hpp"
#include "SceneGraph/Object/GameObject/GameObject.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct VkDirectionalLight
	{
		RenderEngine::SceneGraph::GameObject* gameObject;
		RenderEngine::SceneGraph::DirectionalLight* directionalLight;

		/// default constructor
		VkDirectionalLight() = default;
	};
}

#endif