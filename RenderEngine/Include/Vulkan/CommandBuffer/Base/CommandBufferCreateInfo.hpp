#pragma once

#ifndef RENDERENGINE_COMMANDBUFFERCREATEINFO
#define RENDERENGINE_COMMANDBUFFERCREATEINFO

#include "Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Vulkan
{
	class CommandPool;
	class RenderPass;
	class GraphicsPipeline;
	class FrameBuffer;
	class Window;
	class BufferObject;

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