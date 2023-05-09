#pragma once

#ifndef RENDERENGINE_VKSKYBOX
#define RENDERENGINE_VKSKYBOX

#include "Rendering/Vulkan/Scene/Data/MeshData.hpp"
#include "Rendering/Vulkan/Texture/VkTexture.hpp"
#include "Rendering/Vulkan/Scene/Skybox/VkSkyboxCreateInfo.hpp"
#include "Rendering/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"
#include "Rendering/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorSet.hpp"

namespace RenderEngine::Rendering
{
	class VkSkybox
	{
	private:

		VkSkyboxCreateInfo createInfo;
		GraphicsPipeline graphicsPipeline;
		std::vector<DescriptorSet> descriptorSets;

		void CreateGraphicsPipeline(DescriptorBuffer* _cameraBuffer);

		DescriptorDataList GenerateDefaultVertexShaderDescriptorSet(DescriptorBuffer* _cameraBuffer);

		DescriptorDataList GenerateDefaultFragmentShaderDescriptorSet();

		void CreateDescriptorSet(std::vector<DescriptorDataList> _descriptorDatas);
	public:
		MeshData* meshData;
		VkTexture texture;

		void InitializeSkybox(const VkSkyboxCreateInfo& _createInfo, DescriptorBuffer* _cameraBuffer);
		void Draw(VkCommandBuffer _commandBuffer, int _currentFrame) const;
		/// default constructor
		VkSkybox() = default;
	};
}

#endif