#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINE
#define RENDERENGINE_GRAPHICSPIPELINE

#include "Vulkan/GraphicsPipeline/GraphicsPipelineCreateInfo.hpp"

namespace RenderEngine::Vulkan
{
	class GraphicsPipeline
	{
	private:
		VkDevice logicalDevice;
		Shader vertexShader;
		Shader fragmentShader;
		VkExtent2D swapChainExtent;
		VkFormat swapChainImageFormat;
		RenderPass* renderPass;

		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline =VK_NULL_HANDLE;

	public:
		GraphicsPipeline() = default;
		~GraphicsPipeline() = default;

		static void InitalizeGraphicsPipeline(GraphicsPipelineCreateInfo _createInfo, GraphicsPipeline* _output);
		void Cleanup();

		const VkPipeline& GetGraphicsPipeline() const;
	};

}

#endif