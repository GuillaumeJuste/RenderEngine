#pragma once

#ifndef RENDERENGINE_SWAPCHAINCOMMANDBUFFER
#define RENDERENGINE_SWAPCHAINCOMMANDBUFFER

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/CommandBuffer/Base/CommandBufferBase.hpp"

namespace RenderEngine::Vulkan
{
	class SwapChainCommandBuffer : public CommandBufferBase
	{
	private:
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		VkFence inFlightFence;

		BufferObject* vertexBufferObject;
		BufferObject* indexBufferObject;

		void InitializeSyncObjects();
	public:
		SwapChainCommandBuffer() = default;
		~SwapChainCommandBuffer() = default;

		static void InitializeCommandBuffer(CommandBufferCreateInfo _createInfo, SwapChainCommandBuffer* _output);

		void RecordCommandBuffer(uint32_t imageIndex);
		void Cleanup();

		const VkSemaphore& GetImageAvailableSemaphore() const;
		const VkSemaphore& GetRenderFinishedSemaphore() const;
		const VkFence& GetInFlightFence() const;
	};
}

#endif