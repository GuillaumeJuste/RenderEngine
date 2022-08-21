#pragma once

#ifndef RENDERENGINE_SWAPCHAINCOMMANDBUFFERCREATEINFO
#define RENDERENGINE_SWAPCHAINCOMMANDBUFFERCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/CommandBuffer/Base/CommandBufferCreateInfo.hpp"
#include "Vulkan/SwapChain/SwapChain.hpp"
#include "Vulkan/RenderPass/RenderPass.hpp"
#include "Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Vulkan/FrameBuffer/FrameBuffer.hpp"
#include "Vulkan/BufferObject/BufferObject.hpp"

namespace RenderEngine::Vulkan
{

	struct SwapChainCommandBufferCreateInfo : public CommandBufferCreateInfo
	{
		RenderPass* renderPass;
		GraphicsPipeline* graphicsPipeline;
		FrameBuffer* frameBuffer;
		SwapChain* swapChain;

		BufferObject* vertexBufferObject;
		BufferObject* indexBufferObject;

		SwapChainCommandBufferCreateInfo() = default;
	};
}

#endif