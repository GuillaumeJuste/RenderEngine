#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINEVKCREATEINFO
#define RENDERENGINE_GRAPHICSPIPELINEVKCREATEINFO

#include "Engine/Vulkan/Shader/Shader.hpp"
#include "Engine/Vulkan/RenderPass/RenderPass.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	* @brief struct holding data for GraphicsPipeline Initialization
	*/
	struct GraphicsPipelineVkCreateInfo
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
		GraphicsPipelineVkCreateInfo() = default;
	};
}

#endif