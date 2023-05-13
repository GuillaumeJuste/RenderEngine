#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETLAYOUT
#define RENDERENGINE_DESCRIPTORSETLAYOUT

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Descriptor/Layout/DescriptorSetLayoutVkCreateInfo.hpp"


namespace RenderEngine::Rendering
{
	/**
	 * @brief Class to manage creation and usage of Vulkan DescriptorSetLayout
	*/
	class DescriptorSetLayout
	{
	private:
		VkDevice logicalDevice = VK_NULL_HANDLE;

		VkDescriptorSetLayout descriptorSetLayout;

	public:

		/// default constructor
		DescriptorSetLayout() = default;

		/// default destructor
		~DescriptorSetLayout() = default;

		static void InitializeDescriptorSetLayout(const DescriptorSetLayoutVkCreateInfo& _createInfo, DescriptorSetLayout* _output);

		const VkDescriptorSetLayout& GetDescriptorSetLayout() const;

		void Cleanup();
	};

}

#endif