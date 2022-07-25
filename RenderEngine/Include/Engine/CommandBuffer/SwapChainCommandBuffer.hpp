#pragma once

#ifndef RENDERENGINE_SWAPCHAINCOMMANDBUFFER
#define RENDERENGINE_SWAPCHAINCOMMANDBUFFER

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/CommandBuffer/Base/CommandBufferBase.hpp"

namespace RenderEngine
{

	struct SwapChainCommandBuffer : public CommandBufferBase
	{
		SwapChainCommandBuffer() = default;
		~SwapChainCommandBuffer() = default;

		virtual void RecordCommandBuffer(uint32_t imageIndex);
	};
}

#endif