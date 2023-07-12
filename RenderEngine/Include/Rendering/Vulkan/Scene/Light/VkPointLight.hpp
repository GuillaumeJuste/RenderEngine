#pragma once

#ifndef RENDERENGINE_VKPOINTLIGHT
#define RENDERENGINE_VKPOINTLIGHT

#include "Components/Light/PointLight.hpp"
#include "SceneGraph/Object/GameObject/GameObject.hpp"

namespace RenderEngine::Rendering
{
	struct VkPointLight
	{
		RenderEngine::SceneGraph::GameObject* gameObject;
		RenderEngine::Component::PointLight* pointlight;

		/// default constructor
		VkPointLight() = default;
	};
}

#endif