#pragma once

#ifndef RENDERENGINE_SWAPCHAINCOMMANDBUFFERCREATEINFO
#define RENDERENGINE_SWAPCHAINCOMMANDBUFFERCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/CommandBuffer/Base/CommandBufferVkCreateInfo.hpp"
#include "Engine/Vulkan/SwapChain/SwapChain.hpp"
#include "Engine/Vulkan/RenderPass/RenderPass.hpp"
#include "Engine/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Engine/Vulkan/FrameBuffer/FrameBuffer.hpp"
#include "Engine/Vulkan/BufferObject/BufferObject.hpp"

namespace RenderEngine::Engine::Vulkan
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