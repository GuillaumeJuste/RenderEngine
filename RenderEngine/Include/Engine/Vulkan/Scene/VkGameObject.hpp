#pragma once

#ifndef RENDERENGINE_VKGAMEOBJECT
#define RENDERENGINE_VKGAMEOBJECT

#include "Engine/Vulkan/Scene/VkGameObjectCreateInfo.hpp"
#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/BufferObject/BufferObject.hpp"
#include "Engine/Vulkan/UniformBuffer/UniformBufferObject.hpp"
#include "Engine/Vulkan/Descriptor/Set/DescriptorSet.hpp"

#include "Core/Object/Components/MeshRenderer/MeshRenderer.hpp"

using namespace RenderEngine::Core;

namespace RenderEngine::Engine::Vulkan
{
	class VkGameObject
	{
	private:
		VkGameObjectCreateInfo createInfo;

		MeshRenderer* meshRenderer = nullptr;

		BufferObject vertexBufferObject;
		BufferObject indexBufferObject;

		std::vector<UniformBufferObject> uniformBufferObjects;
		
		DescriptorSet descriptorSet;

		void CreateVertexBufferObject();
		void CreateIndexBufferObject();

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
		const BufferObject& GetUBO(uint32_t _frameIndex) const;

		void Cleanup();

	};

}

#endif