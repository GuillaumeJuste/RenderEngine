#pragma once

#ifndef RENDERENGINE_QUEUEFAMILYINDICES
#define RENDERENGINE_QUEUEFAMILYINDICES

#include "Vulkan/VulkanBaseInclude.hpp"
#include <optional>

namespace RenderEngine
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete();
	};
}

#endif