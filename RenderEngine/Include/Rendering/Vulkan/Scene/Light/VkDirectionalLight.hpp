#pragma once

#ifndef RENDERENGINE_VKDIRECTIONALLIGHT
#define RENDERENGINE_VKDIRECTIONALLIGHT

#include "Components/Light/DirectionalLight.hpp"
#include "SceneGraph/Object/GameObject/GameObject.hpp"

namespace RenderEngine::Rendering
{
	struct VkDirectionalLight
	{
		RenderEngine::SceneGraph::GameObject* gameObject;
		RenderEngine::Component::DirectionalLight* directionalLight;

		/// default constructor
		VkDirectionalLight() = default;
	};
}

#endif