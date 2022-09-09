#pragma once

#ifndef RENDERENGINE_COMMANDPOOL
#define RENDERENGINE_COMMANDPOOL

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/CommandPool/CommandPoolVkCreateInfo.hpp"

namespace RenderEngine::Vulkan
{
	/**
	 * @brief Class to manage creation and usage of Vulkan CommandPool
	*/
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
		static void InitializeCommandPool(const CommandPoolVkCreateInfo& _createInfo, CommandPool* _output);

		/// Get VkCommandPool
		const VkCommandPool& GetCommandPool() const;

		/// Clean up vulkan classes
		void Cleanup();
	};

}

#endif