#pragma once

#ifndef RENDERENGINE_DESCRIPTORSET
#define RENDERENGINE_DESCRIPTORSET

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Descriptor/Set/DescriptorSetVkCreateInfo.hpp"

#include <vector>

namespace RenderEngine::Engine::Vulkan
{
	class DescriptorSet
	{
	private:
		VkDevice logicalDevice = VK_NULL_HANDLE;

		std::vector<VkDescriptorSet> descriptorSets;

	public:

		/// default constructor
		DescriptorSet() = default;

		/// default destructor
		~DescriptorSet() = default;

		static void InitializeDescriptorSet(const DescriptorSetVkCreateInfo& _createInfo, DescriptorSet* _output);

		void Cleanup();

		const VkDescriptorSet& GetFrameDescriptorSet(int _frame) const;
	};

}

#endif