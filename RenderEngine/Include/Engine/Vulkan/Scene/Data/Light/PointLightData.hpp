#pragma once

#ifndef RENDERENGINE_POINTLIGHTDATA
#define RENDERENGINE_POINTLIGHTDATA

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct PointLightData
	{
		bool enable = false;
		alignas(16) Mathlib::Vec3 position;
		alignas(16) Mathlib::Vec3 color;
		float range;
		float ambient;
		float diffuse;
		float specular;

		/// default constructor
		PointLightData() = default;
	};
}

#endif