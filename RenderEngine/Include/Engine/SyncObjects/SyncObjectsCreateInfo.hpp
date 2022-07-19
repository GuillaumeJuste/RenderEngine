#pragma once

#ifndef RENDERENGINE_SYNCOBJECTSCREATEINFO
#define RENDERENGINE_SYNCOBJECTSCREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"

namespace RenderEngine
{
	struct SyncObjectsCreateInfo
	{
		VkDevice logicalDevice;

		SyncObjectsCreateInfo() = default;
	};
}

#endif