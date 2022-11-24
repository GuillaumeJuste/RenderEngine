#include "Engine/Vulkan/Scene/GameObject/VkGameObject.hpp"

#include "Engine/Vulkan/UniformBuffer/UniformBufferData.hpp"
#include "Engine/Vulkan/Scene/Data/Material.hpp"

#include "Misc/Math.hpp"

#include <array>

using namespace RenderEngine::Engine::Vulkan;

VkGameObject::VkGameObject(const VkGameObjectCreateInfo& _createInfo) :
	createInfo{ _createInfo }
{
	meshRenderer = createInfo.gameObject->GetComponent<MeshRenderer>();
	
	CreateDescriptorBufferObjects();
}

void VkGameObject::CreateGraphicsPipeline(DescriptorBuffer* _cameraBuffer, DescriptorBuffer* _pointLightsBuffer, DescriptorBuffer* _directionalLightsBuffer, DescriptorBuffer* _spotLightsBuffer)
{
	if (meshRenderer != nullptr)
	{
		GraphicsPipelineVkCreateInfo gpCreateInfo{};
		gpCreateInfo.logicalDevice = createInfo.logicalDevice;
		gpCreateInfo.renderPass = createInfo.renderpass;
		gpCreateInfo.swapChainExtent = createInfo.swapchain->GetExtent();
		gpCreateInfo.swapChainImageFormat = createInfo.swapchain->GetImageFormat();
		gpCreateInfo.meshRenderer = meshRenderer;
		gpCreateInfo.descriptorSetDatas = GenerateDefaultDescriptorSet();

		GraphicsPipeline::InitalizeGraphicsPipeline(gpCreateInfo, &graphicsPipeline);

		CreateDescriptorSet(_cameraBuffer, _pointLightsBuffer, _directionalLightsBuffer, _spotLightsBuffer);
	}
}

void VkGameObject::CreateDescriptorBufferObjects()
{
	BufferObjectVkCreateInfo uniformBufferCreateInfo;
	uniformBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	uniformBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	uniformBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	uniformBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	uniformBufferCreateInfo.bufferSize = sizeof(UniformBufferData);

	DescriptorBuffer::InitializeDescriptorBuffer(uniformBufferCreateInfo, MAX_FRAMES_IN_FLIGHT,  &uniformBufferObject);

	BufferObjectVkCreateInfo materialBufferCreateInfo;
	materialBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	materialBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	materialBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	materialBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	materialBufferCreateInfo.bufferSize = sizeof(Material);

	DescriptorBuffer::InitializeDescriptorBuffer(uniformBufferCreateInfo, MAX_FRAMES_IN_FLIGHT, &materialBufferObject);
}

std::vector<BaseDescriptorSetData> VkGameObject::GenerateDefaultDescriptorSet()
{
	std::vector<BaseDescriptorSetData> data;

	BaseDescriptorSetData uniformBufferData{};
	uniformBufferData.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uniformBufferData.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uniformBufferData.binding = 0;
	data.push_back(uniformBufferData);

	BaseDescriptorSetData cameraBufferData{};
	cameraBufferData.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	cameraBufferData.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	cameraBufferData.binding = 1;
	data.push_back(cameraBufferData);

	BaseDescriptorSetData textureBufferData{};
	textureBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	textureBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	textureBufferData.binding = 2;
	data.push_back(textureBufferData);

	BaseDescriptorSetData materialBufferData{};
	materialBufferData.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	materialBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	materialBufferData.binding = 3;
	data.push_back(materialBufferData);

	BaseDescriptorSetData pointLightBufferData{};
	pointLightBufferData.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	pointLightBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	pointLightBufferData.binding = 4;
	data.push_back(pointLightBufferData);

	BaseDescriptorSetData specularBufferData{};
	specularBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	specularBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	specularBufferData.binding = 5;
	data.push_back(specularBufferData);

	BaseDescriptorSetData directionalLightBufferData{};
	directionalLightBufferData.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	directionalLightBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	directionalLightBufferData.binding = 6;
	data.push_back(directionalLightBufferData);

	BaseDescriptorSetData spotLightBufferData{};
	spotLightBufferData.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	spotLightBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	spotLightBufferData.binding = 7;
	data.push_back(spotLightBufferData);

	return data;
}

void VkGameObject::CreateDescriptorSet(DescriptorBuffer* _cameraBuffer, DescriptorBuffer* _pointLightsBuffer, DescriptorBuffer* _directionalLightsBuffer, DescriptorBuffer* _spotLightsBuffer)
{
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, graphicsPipeline.GetDescriptorSetLayout().GetDescriptorSetLayout());
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = graphicsPipeline.GetDescriptorPool().GetVkDescriptorPool();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(createInfo.logicalDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBufferObject[i].GetVkBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferData);

		VkDescriptorBufferInfo cameraBufferInfo{};
		cameraBufferInfo.buffer = _cameraBuffer->operator[](i).GetVkBuffer();
		cameraBufferInfo.offset = 0;
		cameraBufferInfo.range = sizeof(CameraBufferData);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = createInfo.textureData->vkTexture.GetImageView();
		imageInfo.sampler = createInfo.textureData->vkTexture.GetSampler();

		VkDescriptorBufferInfo materialBufferInfo{};
		materialBufferInfo.buffer = materialBufferObject[i].GetVkBuffer();
		materialBufferInfo.offset = 0;
		materialBufferInfo.range = sizeof(Material);

		VkDescriptorBufferInfo pointLightBufferInfo{};
		pointLightBufferInfo.buffer = _pointLightsBuffer->operator[](i).GetVkBuffer();
		pointLightBufferInfo.offset = 0;
		pointLightBufferInfo.range = _pointLightsBuffer->operator[](i).GetBufferSize();

		VkDescriptorImageInfo specularSamplerInfo{};
		specularSamplerInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		specularSamplerInfo.imageView = createInfo.specularMap->vkTexture.GetImageView();
		specularSamplerInfo.sampler = createInfo.specularMap->vkTexture.GetSampler();

		VkDescriptorBufferInfo directionalLightBufferInfo{};
		directionalLightBufferInfo.buffer = _directionalLightsBuffer->operator[](i).GetVkBuffer();
		directionalLightBufferInfo.offset = 0;
		directionalLightBufferInfo.range = _directionalLightsBuffer->operator[](i).GetBufferSize();

		VkDescriptorBufferInfo spotLightBufferInfo{};
		spotLightBufferInfo.buffer = _spotLightsBuffer->operator[](i).GetVkBuffer();
		spotLightBufferInfo.offset = 0;
		spotLightBufferInfo.range = _spotLightsBuffer->operator[](i).GetBufferSize();

		std::array<VkWriteDescriptorSet, 8> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pBufferInfo = &cameraBufferInfo;

		descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[2].dstSet = descriptorSets[i];
		descriptorWrites[2].dstBinding = 2;
		descriptorWrites[2].dstArrayElement = 0;
		descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[2].descriptorCount = 1;
		descriptorWrites[2].pImageInfo = &imageInfo;

		descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[3].dstSet = descriptorSets[i];
		descriptorWrites[3].dstBinding = 3;
		descriptorWrites[3].dstArrayElement = 0;
		descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[3].descriptorCount = 1;
		descriptorWrites[3].pBufferInfo = &materialBufferInfo;

		descriptorWrites[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[4].dstSet = descriptorSets[i];
		descriptorWrites[4].dstBinding = 4;
		descriptorWrites[4].dstArrayElement = 0;
		descriptorWrites[4].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorWrites[4].descriptorCount = 1;
		descriptorWrites[4].pBufferInfo = &pointLightBufferInfo;

		descriptorWrites[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[5].dstSet = descriptorSets[i];
		descriptorWrites[5].dstBinding = 5;
		descriptorWrites[5].dstArrayElement = 0;
		descriptorWrites[5].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[5].descriptorCount = 1;
		descriptorWrites[5].pImageInfo = &specularSamplerInfo;

		descriptorWrites[6].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[6].dstSet = descriptorSets[i];
		descriptorWrites[6].dstBinding = 6;
		descriptorWrites[6].dstArrayElement = 0;
		descriptorWrites[6].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorWrites[6].descriptorCount = 1;
		descriptorWrites[6].pBufferInfo = &directionalLightBufferInfo;

		descriptorWrites[7].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[7].dstSet = descriptorSets[i];
		descriptorWrites[7].dstBinding = 7;
		descriptorWrites[7].dstArrayElement = 0;
		descriptorWrites[7].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorWrites[7].descriptorCount = 1;
		descriptorWrites[7].pBufferInfo = &spotLightBufferInfo;

		vkUpdateDescriptorSets(createInfo.logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void VkGameObject::Draw(VkCommandBuffer _commandBuffer, int _currentFrame) const
{
	if (HasMeshRenderer())
	{
		if (meshRenderer->enable)
		{
			vkCmdBindPipeline(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetGraphicsPipeline());

			VkBuffer vertexBuffers[] = { createInfo.meshData->vertexBufferObject.GetVkBuffer() };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(_commandBuffer, 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(_commandBuffer, createInfo.meshData->indexBufferObject.GetVkBuffer(), 0, VK_INDEX_TYPE_UINT16);

			vkCmdBindDescriptorSets(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetGraphicsPipelineLayout(), 0, 1, &descriptorSets[_currentFrame], 0, nullptr);

			vkCmdDrawIndexed(_commandBuffer, static_cast<uint32_t>(meshRenderer->GetMesh()->indices.size()), 1, 0, 0, 0);
		}
	}
}

MeshRenderer* VkGameObject::GetMeshRenderer() const
{
	return meshRenderer;
}

bool VkGameObject::HasMeshRenderer() const
{
	return meshRenderer != nullptr;
}

void VkGameObject::Update(size_t _currentframe)
{
	UniformBufferData uboData{};
	
	uboData.model = createInfo.gameObject->GetWorldTransform().ToMatrixWithScale().Transpose();

	uniformBufferObject.CopyDataToBuffer<UniformBufferData>((int)_currentframe, &uboData, sizeof(UniformBufferData));

	Material material{};
	material.shininess = meshRenderer->shininess;

	materialBufferObject.CopyDataToBuffer<Material>((int)_currentframe, &material, sizeof(Material));
}

void VkGameObject::Cleanup()
{
	uniformBufferObject.Cleanup();
	materialBufferObject.Cleanup();

	graphicsPipeline.Cleanup();
}