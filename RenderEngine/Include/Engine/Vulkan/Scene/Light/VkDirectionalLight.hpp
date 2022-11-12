#pragma once

#ifndef RENDERENGINE_VKDIRECTIONALLIGHT
#define RENDERENGINE_VKDIRECTIONALLIGHT

#include "Core/Components/Light/DirectionalLight.hpp"
#include "Core/Object/GameObject/GameObject.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct VkDirectionalLight
	{
		RenderEngine::Core::GameObject* gameObject;
		RenderEngine::Core::DirectionalLight* directionalLight;

		/// default constructor
		VkDirectionalLight() = default;
	};
}

#endif