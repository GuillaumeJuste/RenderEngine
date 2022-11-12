#pragma once

#ifndef RENDERENGINE_DIRECTIONALLIGHTDATA
#define RENDERENGINE_DIRECTIONALLIGHTDATA

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct DirectionalLightData
	{
		bool enable = false;
		alignas(16) Mathlib::Vec3 color;
		alignas(16) Mathlib::Vec3 direction;
		float ambient;
		float diffuse;
		float specular;

		/// default constructor
		DirectionalLightData() = default;
	};
}

#endif