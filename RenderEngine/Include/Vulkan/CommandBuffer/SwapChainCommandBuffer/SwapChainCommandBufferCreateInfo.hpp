#pragma once

#ifndef RENDERENGINE_SWAPCHAINCOMMANDBUFFERCREATEINFO
#define RENDERENGINE_SWAPCHAINCOMMANDBUFFERCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/CommandBuffer/Base/CommandBufferCreateInfo.hpp"
#include "Vulkan/SwapChain/SwapChain.hpp"

namespace RenderEngine::Vulkan
{

	struct SwapChainCommandBufferCreateInfo : public CommandBufferCreateInfo
	{
		BufferObject* vertexBufferObject;
		BufferObject* indexBufferObject;
		SwapChain* swapChain;

		SwapChainCommandBufferCreateInfo() = default;
	};
}

#endif