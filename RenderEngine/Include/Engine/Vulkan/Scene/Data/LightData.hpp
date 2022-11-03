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
		alignas(16) Mathlib::Vec3 ambient;
		alignas(16) Mathlib::Vec3 diffuse;
		alignas(16) Mathlib::Vec3 specular;

		/// default constructor
		LightData() = default;
	};
}

#endif