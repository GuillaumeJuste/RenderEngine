#pragma once

#ifndef RENDERENGINE_RENDERPASS
#define RENDERENGINE_RENDERPASS

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/RenderPass/RenderPassCreateInfo.hpp"

namespace RenderEngine::Vulkan
{
	class RenderPass
	{
	private:
		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;
		
		/// render pass
		VkRenderPass renderPass;

	public:

		/// default constructor
		RenderPass() = default;

		/// default destructor
		~RenderPass() = default;

		/**
		 * @brief Initiliaze render pass
		 * @param _createInfo render pass create info
		 * @param _output render pass to initilize
		*/
		static void InitializeRenderPass(RenderPassCreateInfo _createInfo, RenderPass* _output);
		
		/**
		 * @brief clean up vulkan classes
		*/
		void Cleanup();

		/// get VkRenderPass
		const VkRenderPass& GetRenderPass() const;
	};

}

#endif