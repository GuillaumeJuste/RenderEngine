#pragma once

#ifndef RENDERENGINE_LIGHTDATA
#define RENDERENGINE_LIGHTDATA

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct LightData
	{
		alignas(16) Mathlib::Vec3 position;
		alignas(16) Mathlib::Vec3 color;
		float range;
		float ambient;
		float diffuse;
		float specular;

		/// default constructor
		LightData() = default;
	};
}

#endif