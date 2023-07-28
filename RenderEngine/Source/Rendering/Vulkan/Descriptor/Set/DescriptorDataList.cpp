#include "Rendering/Vulkan/Descriptor/Set/DescriptorDataList.hpp"
#include "Rendering/Vulkan/Shader/VkShader.hpp"

using namespace RenderEngine::Rendering;

#include <map>

size_t DescriptorDataList::Size() const
{
	return descriptorDatas.size();
}

void DescriptorDataList::Add(const DescriptorData& _data)
{
	descriptorDatas.push_back(_data);
}

const DescriptorData& DescriptorDataList::operator[](size_t _index) const
{
	return descriptorDatas[_index];
}

std::vector<DescriptorDataList> DescriptorDataList::GenerateDescriptorDataLists(DescriptorDataListCreateInfo& _createInfo)
{
	std::vector<std::map< uint32_t, DescriptorData>> descriptorSets;

	for (int descriptorSetIndex = 0; descriptorSetIndex < _createInfo.descriptorSets.size(); descriptorSetIndex++)
	{
		for (std::forward_list<ShaderDescriptor>::iterator it = _createInfo.descriptorSets[descriptorSetIndex].shaderDescriptors.begin(); it != _createInfo.descriptorSets[descriptorSetIndex].shaderDescriptors.end(); ++it)
		{
			size_t setCount = it->set + 1;

			if (descriptorSets.size() < setCount)
				descriptorSets.resize(setCount);

			if (descriptorSets[it->set].count(it->binding))
			{
				descriptorSets[it->set][it->binding].stageFlags = descriptorSets[it->set][it->binding].stageFlags | VkShader::EnumToVkFlag(it->shaderStage);
			}

			else if (it->descriptorType != DescriptorType::UNKNOWN)
			{
				DescriptorData descriptor{};
				descriptor.binding = it->binding;
				descriptor.stageFlags = VkShader::EnumToVkFlag(it->shaderStage);

				if (it->descriptorType <= DescriptorType::MESHRENDERER_MATERIAL_BUFFER)
				{
					descriptor.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					if (it->descriptorType == DescriptorType::UNIFORM_BUFFER && _createInfo.uniformBuffer != nullptr)
						descriptor.buffer = _createInfo.uniformBuffer;
					if (it->descriptorType == DescriptorType::CAMERA_BUFFER && _createInfo.cameraBuffer != nullptr)
						descriptor.buffer = _createInfo.cameraBuffer;
					if (it->descriptorType == DescriptorType::MESHRENDERER_MATERIAL_BUFFER && _createInfo.materialBuffer != nullptr)
						descriptor.buffer = _createInfo.materialBuffer;
				}

				else if (it->descriptorType > DescriptorType::MESHRENDERER_MATERIAL_BUFFER && it->descriptorType <= DescriptorType::CUSTOM_BUFFER)
				{
					descriptor.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
					if (it->descriptorType == DescriptorType::POINT_LIGHT_BUFFER && _createInfo.pointLightBuffer != nullptr)
						descriptor.buffer = _createInfo.pointLightBuffer;
					if (it->descriptorType == DescriptorType::DIRECTIONAL_LIGHT_BUFFER && _createInfo.directionalLightBuffer != nullptr)
						descriptor.buffer = _createInfo.directionalLightBuffer;
					if (it->descriptorType == DescriptorType::SPOT_LIGHT_BUFFER && _createInfo.spotLightBuffer != nullptr)
						descriptor.buffer = _createInfo.spotLightBuffer;
				}

				else
				{
					descriptor.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					if (it->descriptorType == DescriptorType::SKYBOX_CUBEMAP && _createInfo.skyboxMap != nullptr)
						descriptor.texture = _createInfo.skyboxMap;
					else if (it->descriptorType == DescriptorType::SKYBOX_IRRADIANCE && _createInfo.irradianceMap != nullptr)
						descriptor.texture = _createInfo.irradianceMap;
					else if (it->descriptorType == DescriptorType::SKYBOX_PREFILTERED && _createInfo.prefilterMap != nullptr)
						descriptor.texture = _createInfo.prefilterMap;
					else if (it->descriptorType == DescriptorType::SKYBOX_BRDF_LUT && _createInfo.BRDFlut != nullptr)
						descriptor.texture = _createInfo.BRDFlut;
					else if (it->descriptorType == DescriptorType::MESHRENDERER_MATERIAL_ALBEDO && _createInfo.albedo != nullptr)
						descriptor.texture = _createInfo.albedo;
					else if (it->descriptorType == DescriptorType::MESHRENDERER_MATERIAL_METALNESS && _createInfo.metalnessMap != nullptr)
						descriptor.texture = _createInfo.metalnessMap;
					else if (it->descriptorType == DescriptorType::MESHRENDERER_MATERIAL_ROUGHNESS && _createInfo.roughnessMap != nullptr)
						descriptor.texture = _createInfo.roughnessMap;
					else if (it->descriptorType == DescriptorType::MESHRENDERER_MATERIAL_NORMALMAP && _createInfo.normalMap != nullptr)
						descriptor.texture = _createInfo.normalMap;
					else if (it->descriptorType == DescriptorType::MESHRENDERER_MATERIAL_AOMAP && _createInfo.ambientOcclusionMap != nullptr)
						descriptor.texture = _createInfo.ambientOcclusionMap;
					else if (it->descriptorType == DescriptorType::CUSTOM_TEXTURE && it->texture != nullptr)
						descriptor.texture = dynamic_cast<VkTexture*>(it->texture->iTexture);
				}

				if (descriptor.buffer != nullptr || descriptor.texture != nullptr)
					descriptorSets[it->set][it->binding] = descriptor;
			}
		}
	}

	std::vector<DescriptorDataList> output;
	output.resize(descriptorSets.size());
	for (int descriptorSetIndex = 0; descriptorSetIndex < descriptorSets.size(); descriptorSetIndex++)
	{
		for (std::map< uint32_t, DescriptorData>::iterator it = descriptorSets[descriptorSetIndex].begin(); it != descriptorSets[descriptorSetIndex].end(); ++it)
		{
			output[descriptorSetIndex].Add(it->second);
		}
	}
	return output;
}