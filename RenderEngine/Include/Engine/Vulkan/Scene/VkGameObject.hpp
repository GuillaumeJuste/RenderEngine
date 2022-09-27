#pragma once

#ifndef RENDERENGINE_VKGAMEOBJECT
#define RENDERENGINE_VKGAMEOBJECT

#include "Engine/Vulkan/Scene/VkGameObjectCreateInfo.hpp"
#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/BufferObject/BufferObject.hpp"
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
		
		void CreateVertexBufferObject();
		void CreateIndexBufferObject();

	public:
		VkGameObject(const VkGameObjectCreateInfo& _createInfo);
		~VkGameObject() = default;

		MeshRenderer* GetMeshRenderer() const;

		bool HasMeshRenderer() const;

		const BufferObject& GetVBO() const;
		const BufferObject& GetIBO() const;

		void Cleanup();

	};

}

#endif