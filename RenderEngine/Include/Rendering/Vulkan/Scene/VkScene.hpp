#pragma once

#ifndef RENDERENGINE_VKSCENE
#define RENDERENGINE_VKSCENE

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Scene/VkSceneCreateInfo.hpp"
#include "Rendering/Vulkan/Scene/GameObject/VkGameObjectCreateInfo.hpp"
#include "Rendering/Vulkan/Scene/GameObject/VkGameObject.hpp"

#include "Rendering/Vulkan/Scene/Skybox/VkSkybox.hpp"

#include "Rendering/Vulkan/Scene/Data/MeshData.hpp"
#include "Rendering/Vulkan/Scene/Data/TextureData.hpp"
#include "Rendering/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"
#include "Rendering/Vulkan/Scene/Light/VkPointLight.hpp"
#include "Rendering/Vulkan/Scene/Light/VkDirectionalLight.hpp"
#include "Rendering/Vulkan/Scene/Light/VkSpotLight.hpp"
#include "Rendering/Vulkan/Scene/Data/Light/PointLightData.hpp"
#include "Rendering/Vulkan/Scene/Data/Light/DirectionalLightData.hpp"
#include "Rendering/Vulkan/Scene/Data/Light/SpotLightData.hpp"

#include <forward_list>
#include <vector>

namespace RenderEngine::Rendering
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

		TextureData* LoadTexture(RenderEngine::Assets::Texture* _texture, uint32_t _imageArrayLayers = 1, VkImageCreateFlags _imageFlags = 0);
		TextureData* GetTexture(RenderEngine::Assets::Texture* _texture);

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