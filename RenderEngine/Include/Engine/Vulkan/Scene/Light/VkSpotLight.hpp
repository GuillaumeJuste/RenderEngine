#pragma once

#ifndef RENDERENGINE_VKSPOTLIGHT
#define RENDERENGINE_VKSPOTLIGHT

#include "Core/Components/Light/SpotLight.hpp"
#include "Core/Object/GameObject/GameObject.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct VkSpotLight
	{
		RenderEngine::Core::GameObject* gameObject;
		RenderEngine::Core::SpotLight* spotLight;

		/// default constructor
		VkSpotLight() = default;
	};
}

#endif