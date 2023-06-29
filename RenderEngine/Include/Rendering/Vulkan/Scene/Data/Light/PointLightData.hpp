#pragma once

#ifndef RENDERENGINE_POINTLIGHTDATA
#define RENDERENGINE_POINTLIGHTDATA

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Rendering
{
	struct PointLightData
	{
		int enable = false;
		alignas(16) Mathlib::Vec3 position;
		alignas(16) Mathlib::Vec3 color;
		float intensity = 1.0f;
		float range;

		/// default constructor
		PointLightData() = default;
	};
}

#endif