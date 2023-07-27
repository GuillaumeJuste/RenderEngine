#pragma once

#ifndef RENDERENGINE_DESCRIPTORDATALISTCREATEINFO
#define RENDERENGINE_DESCRIPTORDATALISTCREATEINFO

#include "ResourceManager/Assets/Shader/ShaderDescriptorSet.hpp"
using namespace RenderEngine::Assets;

#include "Rendering/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"
#include "Rendering/Vulkan/Texture/VkTexture.hpp"

#include <vector>

namespace RenderEngine::Rendering
{
	/**
	* @brief struct holding data for Descriptor data list Initialization
	*/
	struct DescriptorDataListCreateInfo
	{
		std::vector<ShaderDescriptorSet> descriptorSets;

		DescriptorBuffer* uniformBuffer = nullptr;
		DescriptorBuffer* materialBuffer = nullptr;
		DescriptorBuffer* cameraBuffer = nullptr;
		DescriptorBuffer* pointLightBuffer = nullptr;
		DescriptorBuffer* directionalLightBuffer = nullptr;
		DescriptorBuffer* spotLightBuffer = nullptr;

		VkTexture* skyboxMap = nullptr;
		VkTexture* irradianceMap = nullptr;
		VkTexture* prefilterMap = nullptr;
		VkTexture* BRDFlut = nullptr;

		/// Albedo
		VkTexture* albedo = nullptr;

		/// Metalness map
		VkTexture* metalnessMap = nullptr;

		/// Roughness map
		VkTexture* roughnessMap = nullptr;

		/// Normal map
		VkTexture* normalMap = nullptr;

		/// Ambient Occlusion map
		VkTexture* ambientOcclusionMap = nullptr;

		DescriptorDataListCreateInfo() = default;
	};
}

#endif