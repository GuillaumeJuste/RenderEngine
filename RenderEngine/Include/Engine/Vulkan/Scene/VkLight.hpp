#pragma once

#ifndef RENDERENGINE_VKLIGHT
#define RENDERENGINE_VKLIGHT

#include "Core/Components/Light/PointLight.hpp"
#include "Core/Object/GameObject/GameObject.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct VkLight
	{
		RenderEngine::Core::GameObject* gameObject;
		RenderEngine::Core::PointLight* light;

		/// default constructor
		VkLight() = default;
	};
}

#endif