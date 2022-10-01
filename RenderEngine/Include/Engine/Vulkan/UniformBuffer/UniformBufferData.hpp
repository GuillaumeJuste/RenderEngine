#pragma once

#ifndef RENDERENGINE_UNIFORMBUFFERDATA
#define RENDERENGINE_UNIFORMBUFFERDATA

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Matrix/Mat4.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct UniformBufferData
	{
		Mathlib::Mat4 model;
		Mathlib::Mat4 view;
		Mathlib::Mat4 proj;

		UniformBufferData() = default;
	};
}

#endif