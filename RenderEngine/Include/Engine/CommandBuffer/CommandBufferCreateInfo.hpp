#pragma once

#ifndef RENDERENGINE_COMMANDBUFFERCREATEINFO
#define RENDERENGINE_COMMANDBUFFERCREATEINFO

#include "Vulkan/VulkanBaseInclude.hpp"

namespace RenderEngine
{
	class CommandPool;
	class RenderPass;
	class GraphicsPipeline;
	class FrameBuffer;
	class Window;

	struct CommandBufferCreateInfo
	{
		VkDevice logicalDevice;

		CommandPool* commandPool;
		RenderPass* renderPass;
		GraphicsPipeline* graphicsPipeline;
		FrameBuffer* frameBuffer;
		Window* window;

		CommandBufferCreateInfo() = default;
	};
}

#endif