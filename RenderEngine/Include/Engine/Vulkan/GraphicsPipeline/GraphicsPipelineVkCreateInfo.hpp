#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINEVKCREATEINFO
#define RENDERENGINE_GRAPHICSPIPELINEVKCREATEINFO

#include "Engine/Vulkan/Shader/Shader.hpp"
#include "Engine/Vulkan/RenderPass/RenderPass.hpp"

#include "SceneGraph/Components/MeshRenderer/MeshRenderer.hpp"
#include "Engine/Vulkan/Descriptor/Set/DescriptorDataList.hpp"

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