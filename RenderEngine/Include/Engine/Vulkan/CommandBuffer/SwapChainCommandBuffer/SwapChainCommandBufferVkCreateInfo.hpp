#pragma once

#ifndef RENDERENGINE_SWAPCHAINCOMMANDBUFFERVKCREATEINFO
#define RENDERENGINE_SWAPCHAINCOMMANDBUFFERVKCREATEINFO

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/CommandBuffer/CommandBufferVkCreateInfo.hpp"
#include "Engine/Vulkan/SwapChain/SwapChain.hpp"
#include "Engine/Vulkan/RenderPass/RenderPass.hpp"
#include "Engine/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Engine/Vulkan/FrameBuffer/FrameBuffer.hpp"
#include "Engine/Vulkan/BufferObject/BufferObject.hpp"

#include "Engine/Base/CreateInfo/SwapChainCommandBufferCreateInfo.hpp"
using namespace RenderEngine::Engine::Base;

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for SwapChainCommandBufferObject Initialization
	*/
	struct SwapChainCommandBufferVkCreateInfo : public CommandBufferVkCreateInfo
	{

		SwapChainCommandBufferCreateInfo commandBufferCreateInfo;
		
		/// render pass
		RenderPass* renderPass;

		/// graphics pipeline
		GraphicsPipeline* graphicsPipeline;
		
		/// framebuffer
		FrameBuffer* frameBuffer;
		
		/// swapchain
		SwapChain* swapChain;

		/// default constructor
		SwapChainCommandBufferVkCreateInfo() = default;
	};
}

#endif