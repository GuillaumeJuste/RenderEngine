#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINEVKCREATEINFO
#define RENDERENGINE_GRAPHICSPIPELINEVKCREATEINFO

#include "Rendering/Vulkan/RenderPass/RenderPass.hpp"
#include "Rendering/Vulkan/Shader/VkShader.hpp"
#include "SceneGraph/Components/MeshRenderer/MeshRenderer.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorDataList.hpp"

namespace RenderEngine::Rendering
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

		/// renderpass
		RenderPass* renderPass;

		RenderEngine::SceneGraph::MeshRenderer* meshRenderer;

		std::vector<DescriptorDataList> descriptorDatas;

		/// default constructor
		GraphicsPipelineVkCreateInfo() = default;
	};
}

#endif