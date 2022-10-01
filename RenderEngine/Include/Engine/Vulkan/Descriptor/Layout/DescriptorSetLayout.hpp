#pragma once

#ifndef RENDERENGINE_DESCRIPTORSETLAYOUT
#define RENDERENGINE_DESCRIPTORSETLAYOUT

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Descriptor/Layout/DescriptorSetLayoutVkCreateInfo.hpp"


namespace RenderEngine::Engine::Vulkan
{
	/**
	 * @brief Class to manage creation and usage of Vulkan ImageView
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