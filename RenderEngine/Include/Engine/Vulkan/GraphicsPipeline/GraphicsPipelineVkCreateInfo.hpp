#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINEVKCREATEINFO
#define RENDERENGINE_GRAPHICSPIPELINEVKCREATEINFO

#include "Engine/Vulkan/Shader/Shader.hpp"
#include "Engine/Vulkan/RenderPass/RenderPass.hpp"

#include "Engine/Base/CreateInfo/GraphicsPipelineCreateInfo.hpp"
using namespace RenderEngine::Engine::Base;

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for GraphicsPipeline Initialization
	*/
	struct GraphicsPipelineVkCreateInfo
	{
		/// logical device
		VkDevice logicalDevice;
		
		GraphicsPipelineCreateInfo graphicsPipelineCreateInfo;

		/// swapchain extent
		VkExtent2D swapChainExtent;
		
		/// swapchain image format
		VkFormat swapChainImageFormat;

		/// renderpass
		RenderPass* renderPass;

		/// default constructor
		GraphicsPipelineVkCreateInfo() = default;
	};
}

#endif