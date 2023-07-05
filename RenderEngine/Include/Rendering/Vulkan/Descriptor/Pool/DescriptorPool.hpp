#pragma once

#ifndef RENDERENGINE_DESCRIPTORPOOL
#define RENDERENGINE_DESCRIPTORPOOL

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Descriptor/Pool/DescriptorPoolVkCreateInfo.hpp"


namespace RenderEngine::Rendering
{
	/**
	 * @brief Class to manage creation and usage of Vulkan DescriptorPool
	*/
	class DescriptorPool
	{
	private:
		VkDevice logicalDevice = VK_NULL_HANDLE;

		VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
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