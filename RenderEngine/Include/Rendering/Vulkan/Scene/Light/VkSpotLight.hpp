#pragma once

#ifndef RENDERENGINE_VKSPOTLIGHT
#define RENDERENGINE_VKSPOTLIGHT

#include "Components/Light/SpotLight.hpp"
#include "SceneGraph/Object/GameObject/GameObject.hpp"

namespace RenderEngine::Rendering
{
	struct VkSpotLight
	{
		RenderEngine::SceneGraph::GameObject* gameObject;
		RenderEngine::Component::SpotLight* spotLight;

		/// default constructor
		VkSpotLight() = default;
	};
}

#endif