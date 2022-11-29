#pragma once

#ifndef RENDERENGINE_VKGAMEOBJECT
#define RENDERENGINE_VKGAMEOBJECT

#include "Engine/Vulkan/Scene/GameObject/VkGameObjectCreateInfo.hpp"
#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/BufferObject/BufferObject.hpp"
#include "Engine/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Engine/Vulkan/Descriptor/Set/DescriptorSet.hpp"
#include "Engine/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"
#include "Core/Components/MeshRenderer/MeshRenderer.hpp"
#include "Engine/Vulkan/Texture/VkTexture.hpp"
#include "Engine/Vulkan/Descriptor/Set/DescriptorData.hpp"
#include "Engine/Vulkan/Descriptor/Set/DescriptorDataList.hpp"

using namespace RenderEngine::Core;

namespace RenderEngine::Engine::Vulkan
{
	class VkGameObject
	{
	private:
		VkGameObjectCreateInfo createInfo;

		MeshRenderer* meshRenderer = nullptr;

		GraphicsPipeline graphicsPipeline;

		DescriptorBuffer uniformBufferObject;

		DescriptorBuffer materialBufferObject;

		DescriptorSet descriptorSets;

		DescriptorDataList GenerateDefaultDescriptorSet(DescriptorBuffer* _cameraBuffer, DescriptorBuffer* _pointLightsBuffer, DescriptorBuffer* _directionalLightsBuffer, DescriptorBuffer* _spotLightsBuffer);

		void CreateDescriptorSet(const DescriptorDataList& _descriptorData);

		void CreateDescriptorBufferObjects();


	public:
		VkGameObject(const VkGameObjectCreateInfo& _createInfo);
		~VkGameObject() = default;

		void CreateGraphicsPipeline(DescriptorBuffer* _cameraBuffer, DescriptorBuffer* _pointLightsBuffer, DescriptorBuffer* _directionalLightsBuffer, DescriptorBuffer* _spotLightsBuffer);

		void Draw(VkCommandBuffer _commandBuffer, int _currentFrame) const;

		MeshRenderer* GetMeshRenderer() const;

		bool HasMeshRenderer() const;

		void Update(size_t _currentframe);

		void Cleanup();

	};

}

#endif