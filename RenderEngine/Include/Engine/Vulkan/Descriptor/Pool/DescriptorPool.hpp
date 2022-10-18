#pragma once

#ifndef RENDERENGINE_DESCRIPTORPOOL
#define RENDERENGINE_DESCRIPTORPOOL

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Descriptor/Pool/DescriptorPoolVkCreateInfo.hpp"


namespace RenderEngine::Engine::Vulkan
{
	/**
	 * @brief Class to manage creation and usage of Vulkan ImageView
	*/
	class DescriptorPool
	{
	private:
		VkDevice logicalDevice = VK_NULL_HANDLE;

		VkDescriptorPool descriptorPool;
	public:

		/// default constructor
		DescriptorPool() = default;

		/// default destructor
		~DescriptorPool() = default;

		static void InitializeDescriptorPool(const DescriptorPoolVkCreateInfo& _createInfo, DescriptorPool* _output);

		const VkDescriptorPool& GetVkDescriptorPool() const;

		void Cleanup();
	};

}

#endif