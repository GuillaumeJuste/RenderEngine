#pragma once

#ifndef RENDERENGINE_QUEUEFAMILYINDICES
#define RENDERENGINE_QUEUEFAMILYINDICES

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include <optional>

namespace RenderEngine::Vulkan
{
	struct QueueFamilyIndices
	{
		/**
		 * @brief Index of the graphics queue
		*/
		std::optional<uint32_t> graphicsFamily;

		/**
		 * @brief index of the present queue
		*/
		std::optional<uint32_t> presentFamily;

		/**
		 * @brief Check if both graphic and prensent queue have valid index
		*/
		bool isComplete();
	};
}

#endif