#pragma once

#ifndef RENDERENGINE_MATERIAL
#define RENDERENGINE_MATERIAL

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct Material
	{
		alignas(16) Mathlib::Vec3 specular;
		float shininess;

		/// default constructor
		Material() = default;
	};
}

#endif