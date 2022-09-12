#pragma once

#ifndef RENDERENGINE_COMMANDBUFFERBASE
#define RENDERENGINE_COMMANDBUFFERBASE

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/CommandBuffer/Base/CommandBufferVkCreateInfo.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	 * @brief Base class for command buffer creation
	*/
	class CommandBufferBase
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
		static void InitializeCommandBuffer(const CommandBufferVkCreateInfo& _createInfo, CommandBufferBase* _output);

		/**
		 * @brief record command buffer command
		 * @param imageIndex index of the frame buffer index;
		*/
		virtual void RecordCommandBuffer(uint32_t imageIndex) = 0;

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