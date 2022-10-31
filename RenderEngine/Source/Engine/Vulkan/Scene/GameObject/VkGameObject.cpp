#include "Engine/Vulkan/Scene/GameObject/VkGameObject.hpp"

#include "Engine/Vulkan/UniformBuffer/UniformBufferData.hpp"

#include "Misc/Math.hpp"

#include <array>

using namespace RenderEngine::Engine::Vulkan;

VkGameObject::VkGameObject(const VkGameObjectCreateInfo& _createInfo, DescriptorBuffer* _cameraBuffer) :
	createInfo{ _createInfo }
{

	meshRenderer = createInfo.gameObject->GetComponent<MeshRenderer>();
	if (meshRenderer != nullptr)
	{

		CreateGraphicsPipeline();
	}

	CreateUniformBufferObject();
	
	CreateDescriptorSet(_cameraBuffer);
}

void VkGameObject::CreateGraphicsPipeline()
{
	GraphicsPipelineVkCreateInfo gpCreateInfo{};
	gpCreateInfo.logicalDevice = createInfo.logicalDevice;
	gpCreateInfo.renderPass = createInfo.renderpass;
	gpCreateInfo.swapChainExtent = createInfo.swapchain->GetExtent();
	gpCreateInfo.swapChainImageFormat = createInfo.swapchain->GetImageFormat();
	gpCreateInfo.meshRenderer = meshRenderer;

	GraphicsPipeline::InitalizeGraphicsPipeline(gpCreateInfo, &graphicsPipeline);
}

void VkGameObject::CreateUniformBufferObject()
{
	BufferObjectVkCreateInfo uniformBufferCreateInfo;
	uniformBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	uniformBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	uniformBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	uniformBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	uniformBufferCreateInfo.bufferSize = sizeof(UniformBufferData);

	DescriptorBuffer::InitializeDescriptorBuffer(uniformBufferCreateInfo, MAX_FRAMES_IN_FLIGHT,  &uniformBufferObject);
}

void VkGameObject::CreateDescriptorSet(DescriptorBuffer* _cameraBuffer)
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

		VkDescriptorBufferInfo camerabufferInfo{};
		camerabufferInfo.buffer = _cameraBuffer->operator[](i).GetVkBuffer();
		camerabufferInfo.offset = 0;
		camerabufferInfo.range = sizeof(CameraBufferData);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = createInfo.textureData->vkTexture.GetImageView();
		imageInfo.sampler = createInfo.textureData->vkTexture.GetSampler();

		std::array<VkWriteDescriptorSet, 3> descriptorWrites{};

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
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[2].dstSet = descriptorSets[i];
		descriptorWrites[2].dstBinding = 2;
		descriptorWrites[2].dstArrayElement = 0;
		descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[2].descriptorCount = 1;
		descriptorWrites[2].pBufferInfo = &camerabufferInfo;

		vkUpdateDescriptorSets(createInfo.logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void VkGameObject::Draw(VkCommandBuffer _commandBuffer, int _currentFrame) const
{
	if (HasMeshRenderer())
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
}

void VkGameObject::Cleanup()
{
	uniformBufferObject.Cleanup();

	graphicsPipeline.Cleanup();
}