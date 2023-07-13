#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINE
#define RENDERENGINE_GRAPHICSPIPELINE

#include "Rendering/Vulkan/GraphicsPipeline/GraphicsPipelineVkCreateInfo.hpp"
#include "Rendering/Vulkan/Descriptor/Layout/DescriptorSetLayout.hpp"
#include "Rendering/Vulkan/Descriptor/Pool/DescriptorPool.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorDataList.hpp"
#include "Rendering/Vulkan/Shader/VkShader.hpp"

namespace RenderEngine::Rendering
{
	/**
	 * @brief Class to manage creation and usage of Vulkan VkPipeline as a graphic pipeline
	*/
	class GraphicsPipeline
	{
	private:

		/// logical device
		VkDevice logicalDevice = VK_NULL_HANDLE;

		/// vertex shader
		VkShader* vertexShader = nullptr;

		/// fragment shader
		VkShader* fragmentShader = nullptr;

		/// swapchain extent
		VkExtent2D swapChainExtent;

		/// swapchain image format
		VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;

		/// render pass
		RenderPass* renderPass = nullptr;

		std::vector<DescriptorSetLayout> descriptorSetLayout;
		std::vector<DescriptorPool> descriptorPool;

		/// pipeline layout
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		
		/// pipeline
		VkPipeline graphicsPipeline =VK_NULL_HANDLE;

		/**
		* @brief Get Vertex struct binding description
		* @return Vertex struct binding description
	   */
		static VkVertexInputBindingDescription GetVertexBindingDescription();

		/**
		 * @brief Get Vertex struct attribute description
		 * @return Vertex struct attribute description
		*/
		static std::array<VkVertexInputAttributeDescription, 4> GetVertexAttributeDescriptions();

		void CreateDescriptorPool(std::vector<DescriptorDataList> _descriptorSetDatas);

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
		static void InitalizeGraphicsPipeline(const GraphicsPipelineVkCreateInfo& _createInfo, GraphicsPipeline* _output);

		/**
		 * @brief clean up vulkan classes
		*/
		void Cleanup();

		/// Get Graphics Pipeline
		const VkPipeline& GetGraphicsPipeline() const;

		const VkPipelineLayout& GetGraphicsPipelineLayout() const;

		const DescriptorSetLayout& GetDescriptorSetLayout(size_t _index) const;

		const DescriptorPool& GetDescriptorPool(size_t _index) const;
	};

}

#endif