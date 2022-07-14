#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINE
#define RENDERENGINE_GRAPHICSPIPELINE

#include "Engine/GraphicsPipeline/GraphicsPipelineCreateInfo.hpp"
#include "Engine/RenderPass/RenderPass.hpp"

namespace RenderEngine
{
	class GraphicsPipeline
	{
	private:
		VkDevice* logicalDevice;
		Shader vertexShader;
		Shader fragmentShader;
		VkExtent2D swapChainExtent;
		VkFormat swapChainImageFormat;

		RenderPass renderPass;

		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline =VK_NULL_HANDLE;

		void CreateRenderPass();
		void CreateGraphicsPipeline();

	public:
		GraphicsPipeline() = default;
		~GraphicsPipeline() = default;

		static void InitalizeGraphicsPipeline(GraphicsPipelineCreateInfo _createInfo, GraphicsPipeline* _output);
		void Cleanup();
	};

}

#endif