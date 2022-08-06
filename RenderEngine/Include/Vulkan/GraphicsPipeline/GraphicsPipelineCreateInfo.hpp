#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINECREATEINFO
#define RENDERENGINE_GRAPHICSPIPELINECREATEINFO

#include "Vulkan/Shader/Shader.hpp"
#include "Vulkan/RenderPass/RenderPass.hpp"

namespace RenderEngine::Vulkan
{
	struct GraphicsPipelineCreateInfo
	{
		VkDevice logicalDevice;
		
		VkExtent2D swapChainExtent;
		VkFormat swapChainImageFormat;

		Shader vertexShader;
		Shader fragmentShader;

		RenderPass* renderPass;

		GraphicsPipelineCreateInfo() = default;
	};
}

#endif