#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINECREATEINFO
#define RENDERENGINE_GRAPHICSPIPELINECREATEINFO

#include "Engine/Shader/Shader.hpp"

namespace RenderEngine
{
	struct GraphicsPipelineCreateInfo
	{
		VkDevice* logicalDevice;
		
		
		VkExtent2D swapChainExtent;
		VkFormat swapChainImageFormat;

		Shader vertexShader;
		Shader fragmentShader;

		GraphicsPipelineCreateInfo() = default;
	};
}

#endif