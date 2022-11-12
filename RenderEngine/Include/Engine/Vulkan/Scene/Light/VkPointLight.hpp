#pragma once

#ifndef RENDERENGINE_VKPOINTLIGHT
#define RENDERENGINE_VKPOINTLIGHT

#include "Core/Components/Light/PointLight.hpp"
#include "Core/Object/GameObject/GameObject.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct VkPointLight
	{
		RenderEngine::Core::GameObject* gameObject;
		RenderEngine::Core::PointLight* pointlight;

		/// default constructor
		VkPointLight() = default;
	};
}

#endif