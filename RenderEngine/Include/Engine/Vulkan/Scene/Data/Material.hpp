#pragma once

#ifndef RENDERENGINE_MATERIAL
#define RENDERENGINE_MATERIAL

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct Material
	{
		float shininess;

		/// default constructor
		Material() = default;
	};
}

#endif