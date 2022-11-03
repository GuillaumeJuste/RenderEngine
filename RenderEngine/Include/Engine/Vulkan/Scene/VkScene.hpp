#pragma once

#ifndef RENDERENGINE_VKSCENE
#define RENDERENGINE_VKSCENE

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Scene/VkSceneCreateInfo.hpp"
#include "Engine/Vulkan/Scene/GameObject/VkGameObjectCreateInfo.hpp"
#include "Engine/Vulkan/Scene/GameObject/VkGameObject.hpp"

#include "Engine/Vulkan/Scene/Data/MeshData.hpp"
#include "Engine/Vulkan/Scene/Data/TextureData.hpp"
#include "Engine/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"
#include "Engine/Vulkan/Scene/VkLight.hpp"
#include "Engine/Vulkan/Scene/Data/LightData.hpp"

#include <forward_list>
#include <vector>

namespace RenderEngine::Engine::Vulkan
{
	class VkScene
	{
	private:
		VkSceneCreateInfo createInfo;

		DescriptorBuffer cameraBuffer;
		DescriptorBuffer lightsBuffer;

		std::forward_list<VkGameObject> gameObjects;

		std::vector<MeshData*> sceneMeshes;
		std::vector<TextureData*> sceneTextures;
		std::vector<VkLight> sceneLights;

		void CreateVkGameObjects(VkGameObjectCreateInfo _createInfo, std::vector<GameObject*> _childrens);

		std::vector<LightData> GenerateLightsData();
		void CreateLightBuffer();


		MeshData* LoadMesh(RenderEngine::Core::Mesh* _mesh);
		MeshData* GetMesh(RenderEngine::Core::Mesh* _mesh);
		void CreateVertexBufferObject(RenderEngine::Core::Mesh* _mesh, MeshData* _output);
		void CreateIndexBufferObject(RenderEngine::Core::Mesh* _mesh, MeshData* _output);

		TextureData* LoadTexture(RenderEngine::Core::Texture* _texture);
		TextureData* GetTexture(RenderEngine::Core::Texture* _texture);

		void CreateVkTexture(RenderEngine::Core::Texture* _texture, TextureData* _output);

	public:
		VkScene() = default;
		VkScene(const VkSceneCreateInfo& _createInfo);
		~VkScene() = default;

		void Update(size_t _currentframe);

		void Draw(VkCommandBuffer _commandBuffer, int _currentFrame);

		void Cleanup();
	};

}

#endif