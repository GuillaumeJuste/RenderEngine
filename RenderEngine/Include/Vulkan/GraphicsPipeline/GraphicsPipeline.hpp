#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINE
#define RENDERENGINE_GRAPHICSPIPELINE

#include "Vulkan/GraphicsPipeline/GraphicsPipelineCreateInfo.hpp"

namespace RenderEngine::Vulkan
{
	/**
	 * @brief Class to manage creation and usage of Vulkan VkPipeline as a graphic pipeline
	*/
	class GraphicsPipeline
	{
	private:

		/// logical device
		VkDevice logicalDevice;

		/// vertex shader
		Shader vertexShader;

		/// fragment shader
		Shader fragmentShader;

		/// swapchain extent
		VkExtent2D swapChainExtent;

		/// swapchain image format
		VkFormat swapChainImageFormat;

		/// render pass
		RenderPass* renderPass;

		/// pipeline layout
		VkPipelineLayout pipelineLayout;
		
		/// pipeline
		VkPipeline graphicsPipeline =VK_NULL_HANDLE;

	public:
		/// default constructor
		GraphicsPipeline() = default;
		
		/// default destructor
		~GraphicsPipeline() = default;

		/**
		 * @brief Intilialize graphics pipeline
		 * @param _createInfo graphic piepline create info
		 * @param _output graphic pipeline to initialize
		*/
		static void InitalizeGraphicsPipeline(const GraphicsPipelineCreateInfo& _createInfo, GraphicsPipeline* _output);

		/**
		 * @brief clean up vulkan classes
		*/
		void Cleanup();

		/// Get Graphics Pipeline
		const VkPipeline& GetGraphicsPipeline() const;
	};

}

#endif