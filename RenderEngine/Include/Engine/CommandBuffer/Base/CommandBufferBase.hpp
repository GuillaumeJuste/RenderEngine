#pragma once

#ifndef RENDERENGINE_COMMANDBUFFERBASE
#define RENDERENGINE_COMMANDBUFFERBASE

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/CommandBuffer/CommandBufferCreateInfo.hpp"

namespace RenderEngine
{

	class CommandBufferBase
	{
	protected:
		VkDevice logicalDevice;

		CommandPool* commandPool;
		RenderPass* renderPass;
		GraphicsPipeline* graphicsPipeline;
		FrameBuffer* frameBuffer;
		VkExtent2D swapChainExtent;

		VkCommandBuffer commandBuffer;

	public:
		static void InitializeCommandBuffer(CommandBufferCreateInfo _createInfo, CommandBufferBase* _output);

		virtual void RecordCommandBuffer(uint32_t imageIndex) = 0;
		virtual void Cleanup() = 0;

		const VkCommandBuffer& GetVKCommandBuffer() const;
	};
}

#endif