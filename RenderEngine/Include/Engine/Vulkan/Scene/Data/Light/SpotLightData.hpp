#pragma once

#ifndef RENDERENGINE_SPOTLIGHTDATA
#define RENDERENGINE_SPOTLIGHTDATA

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct SpotLightData
	{
		bool enable = false;
		alignas(16) Mathlib::Vec3 position;
		alignas(16) Mathlib::Vec3 color;
		alignas(16) Mathlib::Vec3 direction;
		float range;
		float cutOff;

		/// default constructor
		SpotLightData() = default;
	};
}

#endif