#pragma once

#ifndef RENDERENGINE_VKPOINTLIGHT
#define RENDERENGINE_VKPOINTLIGHT

#include "SceneGraph/Components/Light/PointLight.hpp"
#include "SceneGraph/Object/GameObject/GameObject.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct VkPointLight
	{
		RenderEngine::SceneGraph::GameObject* gameObject;
		RenderEngine::SceneGraph::PointLight* pointlight;

		/// default constructor
		VkPointLight() = default;
	};
}

#endif