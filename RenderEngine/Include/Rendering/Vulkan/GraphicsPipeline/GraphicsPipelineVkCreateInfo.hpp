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

		std::vector<DescriptorDataList> descriptorDatas;

		bool enableDepthTest = true;
		bool writeDepthBuffer = true;

		VkCullModeFlagBits culling_mode = VK_CULL_MODE_BACK_BIT;

		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;

		Shader* vertexShader;
		Shader* fragmentShader;
		FrontFace frontFace = FrontFace::CLOCKWISE;
		PolygonDrawMode drawMode = PolygonDrawMode::FILL;
		float lineWidth = 1.0f;

		/// default constructor
		GraphicsPipelineVkCreateInfo() = default;
	};
}

#endif