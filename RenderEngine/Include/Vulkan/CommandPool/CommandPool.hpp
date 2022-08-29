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
		/// logical device
		VkDevice logicalDevice;

		/// index of the graphics queue
		uint32_t graphicsQueueIndex;

		/// command pool
		VkCommandPool commandPool;


	public:
		/// default constructor
		CommandPool() = default;

		/// default destructor
		~CommandPool() = default;

		/**
		 * @brief Initialize function
		 * @param _createInfo command pool creation info
		 * @param _output command pool to initilaize
		*/
		static void InitializeCommandPool(CommandPoolCreateInfo _createInfo, CommandPool* _output);

		/// Get VkCommandPool
		const VkCommandPool& GetCommandPool() const;

		/// Clean up vulkan classes
		void Cleanup();
	};

}

#endif