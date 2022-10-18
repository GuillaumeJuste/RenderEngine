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

using namespace RenderEngine::Core;

namespace RenderEngine::Engine::Vulkan
{
	class VkGameObject
	{
	private:
		VkGameObjectCreateInfo createInfo;

		MeshRenderer* meshRenderer = nullptr;

		GraphicsPipeline graphicsPipeline;

		BufferObject vertexBufferObject;
		BufferObject indexBufferObject;

		DescriptorBuffer uniformBufferObject;


		//DescriptorSet descriptorSet;

		std::vector<VkDescriptorSet> descriptorSets;

		VkTexture vkTexture;

		void CreateGraphicsPipeline();

		void CreateVertexBufferObject();
		void CreateIndexBufferObject();

		void CreateVkTexture();

		void CreateUniformBufferObject();

		void CreateDescriptorSet();

	public:
		VkGameObject(const VkGameObjectCreateInfo& _createInfo);
		~VkGameObject() = default;

		void Draw(VkCommandBuffer _commandBuffer, int _currentFrame) const;

		MeshRenderer* GetMeshRenderer() const;

		bool HasMeshRenderer() const;

		const BufferObject& GetVBO() const;
		const BufferObject& GetIBO() const;

		void Update(size_t _currentframe);

		void Cleanup();

	};

}

#endif