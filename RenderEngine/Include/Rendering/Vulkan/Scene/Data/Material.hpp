#pragma once

#ifndef RENDERENGINE_MATERIAL
#define RENDERENGINE_MATERIAL

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Rendering
{
	struct Material
	{
		alignas(16) float shininess;
		alignas(16) Mathlib::Vec3 ambient;
		alignas(16) Mathlib::Vec3 diffuse;
		alignas(16) Mathlib::Vec3 specular;

		/// default constructor
		Material() = default;
	};
}

#endif