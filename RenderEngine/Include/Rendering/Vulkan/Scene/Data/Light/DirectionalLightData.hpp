#pragma once

#ifndef RENDERENGINE_DIRECTIONALLIGHTDATA
#define RENDERENGINE_DIRECTIONALLIGHTDATA

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Rendering
{
	struct DirectionalLightData
	{
		alignas(16) Mathlib::Vec3 color;
		alignas(16) Mathlib::Vec3 direction;
		float intensity = 1.0f;
		int enable = false;

		/// default constructor
		DirectionalLightData() = default;
	};
}

#endif