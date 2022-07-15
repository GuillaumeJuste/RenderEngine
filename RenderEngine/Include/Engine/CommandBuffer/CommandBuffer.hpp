#pragma once

#ifndef RENDERENGINE_COMMANDBUFFER
#define RENDERENGINE_COMMANDBUFFER

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/CommandBuffer/CommandBufferCreateInfo.hpp"

namespace RenderEngine
{

	class CommandBuffer
	{
	private:
		VkDevice* logicalDevice;

		CommandPool* commandPool;
		RenderPass* renderPass;
		GraphicsPipeline* graphicsPipeline;
		FrameBuffer* frameBuffer;
		VkExtent2D swapChainExtent;

		VkCommandBuffer commandBuffer;
	public:
		CommandBuffer() = default;
		~CommandBuffer() = default;

		static void InitializeCommandBuffer(CommandBufferCreateInfo _createInfo, CommandBuffer* _output);

		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	};
}

#endif