#pragma once

#ifndef RENDERENGINE_SWAPCHAINCOMMANDBUFFERVKCREATEINFO
#define RENDERENGINE_SWAPCHAINCOMMANDBUFFERVKCREATEINFO

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/CommandBuffer/CommandBufferVkCreateInfo.hpp"
#include "Rendering/Vulkan/SwapChain/SwapChain.hpp"
#include "Rendering/Vulkan/RenderPass/RenderPass.hpp"
#include "Rendering/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Rendering/Vulkan/FrameBuffer/FrameBuffer.hpp"
#include "Rendering/Vulkan/BufferObject/BufferObject.hpp"

#include "Rendering/Base/CreateInfo/SwapChainCommandBufferCreateInfo.hpp"

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for SwapChainCommandBufferObject Initialization
	*/
	struct SwapChainCommandBufferVkCreateInfo : public CommandBufferVkCreateInfo
	{

		SwapChainCommandBufferCreateInfo commandBufferCreateInfo;
		
		/// render pass
		RenderPass* renderPass;
		
		/// framebuffer
		FrameBuffer* frameBuffer;
		
		/// swapchain
		SwapChain* swapChain;

		/// default constructor
		SwapChainCommandBufferVkCreateInfo() = default;
	};
}

#endif