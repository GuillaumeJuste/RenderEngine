#pragma once

#ifndef RENDERENGINE_COMMANDPOOL
#define RENDERENGINE_COMMANDPOOL

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/CommandPool/CommandPoolCreateInfo.hpp"

namespace RenderEngine::Vulkan
{

	class CommandPool
	{
	private:
		VkDevice logicalDevice;
		uint32_t graphicsQueueIndex;

		VkCommandPool commandPool;


	public:
		CommandPool() = default;
		~CommandPool() = default;

		static void InitializeCommandPool(CommandPoolCreateInfo _createInfo, CommandPool* _output);

		const VkCommandPool& GetCommandPool() const;

		void Cleanup();
	};

}

#endif