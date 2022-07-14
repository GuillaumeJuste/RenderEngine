#pragma once

#ifndef RENDERENGINE_RENDERPASS
#define RENDERENGINE_RENDERPASS

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/RenderPass/RenderPassCreateInfo.hpp"

namespace RenderEngine
{
	class RenderPass
	{
	private:
		VkDevice* logicalDevice = VK_NULL_HANDLE;
		VkRenderPass renderPass;

	public:
		RenderPass() = default;
		~RenderPass() = default;

		static void InitializeRenderPass(RenderPassCreateInfo _createInfo, RenderPass* _output);
		void Cleanup();

		const VkRenderPass& GetRenderPass() const;
	};

}

#endif