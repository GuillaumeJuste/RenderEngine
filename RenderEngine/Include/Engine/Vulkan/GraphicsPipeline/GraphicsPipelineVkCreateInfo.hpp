#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINEVKCREATEINFO
#define RENDERENGINE_GRAPHICSPIPELINEVKCREATEINFO

#include "Engine/Vulkan/Shader/Shader.hpp"
#include "Engine/Vulkan/RenderPass/RenderPass.hpp"

#include "Core/Components/MeshRenderer/MeshRenderer.hpp"
#include "Engine/Vulkan/Descriptor/Set/DescriptorData.hpp"

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

		RenderEngine::Core::MeshRenderer* meshRenderer;

		std::vector<DescriptorData> descriptorDatas;

		/// default constructor
		GraphicsPipelineVkCreateInfo() = default;
	};
}

#endif