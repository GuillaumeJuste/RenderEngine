#pragma once

#ifndef RENDERENGINE_VKGAMEOBJECT
#define RENDERENGINE_VKGAMEOBJECT

#include "Rendering/Vulkan/Scene/GameObject/VkGameObjectCreateInfo.hpp"
#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/BufferObject/BufferObject.hpp"
#include "Rendering/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorSet.hpp"
#include "Rendering/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"
#include "SceneGraph/Components/MeshRenderer/MeshRenderer.hpp"
#include "Rendering/Vulkan/Texture/VkTexture.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorData.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorDataList.hpp"

using namespace RenderEngine::SceneGraph;

namespace RenderEngine::Rendering
{
	class VkGameObject
	{
	private:
		VkGameObjectCreateInfo createInfo;

		MeshRenderer* meshRenderer = nullptr;
		BufferObject* VBO = nullptr;
		BufferObject* IBO = nullptr;

		GraphicsPipeline graphicsPipeline;

		DescriptorBuffer uniformBufferObject;

		DescriptorBuffer materialBufferObject;

		std::vector<DescriptorSet> descriptorSets;

		DescriptorDataList GenerateDefaultVertexShaderDescriptorSet(DescriptorBuffer* _cameraBuffer);

		DescriptorDataList GenerateDefaultFragmentShaderDescriptorSet(DescriptorBuffer* _pointLightsBuffer, DescriptorBuffer* _directionalLightsBuffer, DescriptorBuffer* _spotLightsBuffer, VkTexture* _skybox);

		void CreateDescriptorSet(std::vector<DescriptorDataList> _descriptorDatas);

		void CreateDescriptorBufferObjects();


	public:
		VkGameObject(const VkGameObjectCreateInfo& _createInfo);
		~VkGameObject() = default;

		void CreateGraphicsPipeline(DescriptorBuffer* _cameraBuffer, DescriptorBuffer* _pointLightsBuffer, DescriptorBuffer* _directionalLightsBuffer, DescriptorBuffer* _spotLightsBuffer, VkTexture* _skybox);

		void Draw(VkCommandBuffer _commandBuffer, int _currentFrame) const;

		MeshRenderer* GetMeshRenderer() const;

		bool HasMeshRenderer() const;

		void Update(size_t _currentframe);

		void Cleanup();

	};

}

#endif