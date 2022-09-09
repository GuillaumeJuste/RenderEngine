#pragma once

#ifndef RENDERENGINE_SWAPCHAINCOMMANDBUFFERCREATEINFO
#define RENDERENGINE_SWAPCHAINCOMMANDBUFFERCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/CommandBuffer/Base/CommandBufferVkCreateInfo.hpp"
#include "Vulkan/SwapChain/SwapChain.hpp"
#include "Vulkan/RenderPass/RenderPass.hpp"
#include "Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Vulkan/FrameBuffer/FrameBuffer.hpp"
#include "Vulkan/BufferObject/BufferObject.hpp"

namespace RenderEngine::Vulkan
{
	/**
	* @brief struct holding data for SwapChainCommandBufferObject Initialization
	*/
	struct SwapChainCommandBufferVkCreateInfo : public CommandBufferVkCreateInfo
	{
		/// render pass
		RenderPass* renderPass;

		/// graphics pipeline
		GraphicsPipeline* graphicsPipeline;
		
		/// framebuffer
		FrameBuffer* frameBuffer;
		
		/// swapchain
		SwapChain* swapChain;

		///TMP variables
		BufferObject* vertexBufferObject;
		BufferObject* indexBufferObject;

		/// default constructor
		SwapChainCommandBufferVkCreateInfo() = default;
	};
}

#endif