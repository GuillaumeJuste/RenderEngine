#pragma once

#ifndef RENDERENGINE_COMMANDBUFFER
#define RENDERENGINE_COMMANDBUFFER

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/CommandBuffer/CommandBufferVkCreateInfo.hpp"

namespace RenderEngine::Rendering
{
	/**
	 * @brief Base class for command buffer creation
	*/
	class CommandBuffer
	{
	protected:

		/**
		 * @brief logical device handle
		*/
		VkDevice logicalDevice;

		/**
		 * @brief command pool owning the command buffer
		*/
		CommandPool* commandPool;

		/**
		 * @brief command buffer handle
		*/
		VkCommandBuffer commandBuffer;

	public:

		/**
		 * @brief Initalize command buffer from createInfo
		 * @param _createInfo information to use to initialize command buffer
		 * @param _output command buffer to initialize
		*/
		static void InitializeCommandBuffer(const CommandBufferVkCreateInfo& _createInfo, CommandBuffer* _output);

		static VkCommandBuffer BeginSingleTimeCommands(VkDevice _logicalDevice, CommandPool* _commandPool);

		static void EndSingleTimeCommands(VkDevice _logicalDevice, CommandPool* _commandPool, VkQueue _queue, VkCommandBuffer _commandBuffer);
		
		/**
		 * @brief cleanup function
		*/
		virtual void Cleanup() = 0;

		/**
		 * @brief get command buffer handle
		 * @return command buffer handle
		*/
		const VkCommandBuffer& GetVKCommandBuffer() const;
	};
}

#endif