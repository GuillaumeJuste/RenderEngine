#pragma once

#ifndef RENDERENGINE_VKGAMEOBJECT
#define RENDERENGINE_VKGAMEOBJECT

#include "Rendering/Vulkan/Scene/GameObject/VkGameObjectCreateInfo.hpp"
#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/BufferObject/BufferObject.hpp"
#include "Rendering/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorSet.hpp"
#include "Rendering/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"
#include "Components/MeshRenderer/MeshRenderer.hpp"
#include "Rendering/Vulkan/Texture/VkTexture.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorData.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorDataList.hpp"

using namespace RenderEngine::Component;

namespace RenderEngine::Rendering
{
	enum RenderType
	{
		NORMAL,
		DEPTH_ONLY
	};

	class VkGameObject
	{
	private:
		VkGameObjectCreateInfo createInfo;

		MeshRenderer* meshRenderer = nullptr;
		BufferObject* VBO = nullptr;
		BufferObject* IBO = nullptr;

		GraphicsPipeline graphicsPipeline;
		std::vector<DescriptorSet> descriptorSets;

		GraphicsPipeline shadowGraphicsPipeline;
		std::vector<DescriptorSet> shadowDescriptorSets;

		DescriptorBuffer uniformBufferObject;
		DescriptorBuffer materialBufferObject;


		void CreateDescriptorSet(GraphicsPipeline& _pipeline, std::vector<DescriptorDataList> _descriptorDatas, std::vector<DescriptorSet>& _output);

		void CreateDescriptorBufferObjects();

		void CreateGraphicsPipeline(GraphicsPipeline& _outputPipeline, std::vector<DescriptorSet>& _output, DescriptorBuffer* _cameraBuffer, bool _useFragmentShader);

	public:
		VkGameObject(const VkGameObjectCreateInfo& _createInfo);
		~VkGameObject() = default;


		void Draw(VkCommandBuffer _commandBuffer, int _currentFrame, RenderType _renderType) const;

		MeshRenderer* GetMeshRenderer() const;

		bool HasMeshRenderer() const;

		void Update(size_t _currentframe);

		void Cleanup();

	};

}

#endif