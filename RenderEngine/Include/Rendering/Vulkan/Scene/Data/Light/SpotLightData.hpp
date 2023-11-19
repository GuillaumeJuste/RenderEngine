#pragma once

#ifndef RENDERENGINE_SPOTLIGHTDATA
#define RENDERENGINE_SPOTLIGHTDATA

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Rendering
{
	struct SpotLightData
	{
		alignas(16) Mathlib::Vec3 position;
		alignas(16) Mathlib::Vec3 color;
		alignas(16) Mathlib::Vec3 direction;
		float intensity = 1.0f;
		float range;
		float cutOff;
		int enable = false;

		/// default constructor
		SpotLightData() = default;
	};
}

#endif