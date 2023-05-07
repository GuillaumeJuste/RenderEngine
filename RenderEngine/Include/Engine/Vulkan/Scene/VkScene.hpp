#pragma once

#ifndef RENDERENGINE_VKSCENE
#define RENDERENGINE_VKSCENE

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/Scene/VkSceneCreateInfo.hpp"
#include "Engine/Vulkan/Scene/GameObject/VkGameObjectCreateInfo.hpp"
#include "Engine/Vulkan/Scene/GameObject/VkGameObject.hpp"

#include "Engine/Vulkan/Scene/Skybox/VkSkybox.hpp"

#include "Engine/Vulkan/Scene/Data/MeshData.hpp"
#include "Engine/Vulkan/Scene/Data/TextureData.hpp"
#include "Engine/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"
#include "Engine/Vulkan/Scene/Light/VkPointLight.hpp"
#include "Engine/Vulkan/Scene/Light/VkDirectionalLight.hpp"
#include "Engine/Vulkan/Scene/Light/VkSpotLight.hpp"
#include "Engine/Vulkan/Scene/Data/Light/PointLightData.hpp"
#include "Engine/Vulkan/Scene/Data/Light/DirectionalLightData.hpp"
#include "Engine/Vulkan/Scene/Data/Light/SpotLightData.hpp"

#include <forward_list>
#include <vector>

namespace RenderEngine::Engine::Vulkan
{
	class VkScene
	{
	private:
		VkSceneCreateInfo createInfo;

		int minimumLightCount = 10;

		DescriptorBuffer cameraBuffer;
		DescriptorBuffer pointLightsBuffer;
		DescriptorBuffer directionalLightsBuffer;
		DescriptorBuffer spotLightsBuffer;

		std::forward_list<VkGameObject> gameObjects;

		std::vector<MeshData*> sceneMeshes;
		std::vector<TextureData*> sceneTextures;
		std::vector<VkPointLight> scenePointLights;
		std::vector<VkDirectionalLight> sceneDirectionalLights;
		std::vector<VkSpotLight> sceneSpotLights;
		VkSkybox skybox;

		void CreateVkGameObjects(VkGameObjectCreateInfo _createInfo, std::vector<GameObject*> _childrens);

		std::vector<PointLightData> GeneratePointLightsData();
		std::vector<DirectionalLightData> GenerateDirectionalLightsData();
		std::vector<SpotLightData> GenerateSpotLightsData();
		void CreateLightBuffer();


		MeshData* LoadMesh(RenderEngine::Core::Mesh* _mesh);
		MeshData* GetMesh(RenderEngine::Core::Mesh* _mesh);
		void CreateVertexBufferObject(RenderEngine::Core::Mesh* _mesh, MeshData* _output);
		void CreateIndexBufferObject(RenderEngine::Core::Mesh* _mesh, MeshData* _output);

		TextureData* LoadTexture(RenderEngine::Core::Texture* _texture, uint32_t _imageArrayLayers = 1, VkImageCreateFlags _imageFlags = 0);
		TextureData* GetTexture(RenderEngine::Core::Texture* _texture);

		void CreateSkybox();

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