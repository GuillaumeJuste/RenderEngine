#pragma once

#ifndef RENDERENGINE_QUEUEFAMILYINDICES
#define RENDERENGINE_QUEUEFAMILYINDICES

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include <optional>

namespace RenderEngine::Rendering
{
	/**
	 * @brief struct holding indices of the graphics and present queue for a physical device
	*/
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