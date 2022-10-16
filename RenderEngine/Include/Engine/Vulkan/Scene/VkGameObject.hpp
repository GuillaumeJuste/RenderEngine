#pragma once

#ifndef RENDERENGINE_VKGAMEOBJECT
#define RENDERENGINE_VKGAMEOBJECT

#include "Engine/Vulkan/Scene/VkGameObjectCreateInfo.hpp"
#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/BufferObject/BufferObject.hpp"
#include "Engine/Vulkan/Descriptor/Pool/DescriptorPool.hpp"
#include "Engine/Vulkan/Descriptor/Set/DescriptorSet.hpp"
#include "Engine/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"
#include "Core/Components/MeshRenderer/MeshRenderer.hpp"
#include "Core/Components/Material/Material.hpp"
#include "Engine/Vulkan/Texture/VkTexture.hpp"

using namespace RenderEngine::Core;

namespace RenderEngine::Engine::Vulkan
{
	class VkGameObject
	{
	private:
		VkGameObjectCreateInfo createInfo;

		MeshRenderer* meshRenderer = nullptr;
		Material* material = nullptr;

		BufferObject vertexBufferObject;
		BufferObject indexBufferObject;

		DescriptorBuffer uniformBufferObject;

		DescriptorPool descriptorPool;

		DescriptorSet descriptorSet;

		VkTexture vkTexture;

		void CreateVertexBufferObject();
		void CreateIndexBufferObject();

		void CreateUniformBufferObject();

		void CreateDescriptorPool();
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