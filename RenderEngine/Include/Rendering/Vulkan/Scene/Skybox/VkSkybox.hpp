#pragma once

#ifndef RENDERENGINE_VKSKYBOX
#define RENDERENGINE_VKSKYBOX

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

		BufferObject* vbo = nullptr;
		BufferObject* ibo = nullptr;
		VkTexture* cubemap = nullptr;
		VkTexture* irradiance = nullptr;
		VkTexture* prefilter = nullptr;
		VkTexture* BRDFlut = nullptr;

		void CreateGraphicsPipeline(DescriptorBuffer* _cameraBuffer);

		void CreateDescriptorSet(std::vector<DescriptorDataList> _descriptorDatas);
	public:

		void InitializeSkybox(const VkSkyboxCreateInfo& _createInfo, DescriptorBuffer* _cameraBuffer);
		void Draw(VkCommandBuffer _commandBuffer, int _currentFrame) const;
		/// default constructor
		VkSkybox() = default;

		void Clean();

		VkTexture* GetCubemap() const;
		VkTexture* GetIrradianceCubeMap() const;
		VkTexture* GetPrefilterCubemap() const;
		VkTexture* GetBRDFlut() const;
	};
}

#endif