#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINECREATEINFO
#define RENDERENGINE_GRAPHICSPIPELINECREATEINFO

#include "Vulkan/Shader/Shader.hpp"
#include "Vulkan/RenderPass/RenderPass.hpp"

namespace RenderEngine::Vulkan
{
	struct GraphicsPipelineCreateInfo
	{
		/// logical device
		VkDevice logicalDevice;
		
		/// swapchain extent
		VkExtent2D swapChainExtent;
		
		/// swapchain image format
		VkFormat swapChainImageFormat;

		/// vertex shader
		Shader vertexShader;

		/// fragment shader
		Shader fragmentShader;

		/// renderpass
		RenderPass* renderPass;

		/// default constructor
		GraphicsPipelineCreateInfo() = default;
	};
}

#endif