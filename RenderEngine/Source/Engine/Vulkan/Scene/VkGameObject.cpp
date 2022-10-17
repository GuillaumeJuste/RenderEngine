#include "Engine/Vulkan/Scene/VkGameObject.hpp"

#include "Engine/Vulkan/UniformBuffer/UniformBufferData.hpp"

#include "Misc/Math.hpp"

#include <array>

using namespace RenderEngine::Engine::Vulkan;

VkGameObject::VkGameObject(const VkGameObjectCreateInfo& _createInfo) :
	createInfo{ _createInfo }
{

	meshRenderer = createInfo.gameObject->GetComponent<MeshRenderer>();
	if (meshRenderer != nullptr)
	{
		CreateGraphicsPipeline();
		CreateVertexBufferObject();
		CreateIndexBufferObject();

		CreateVkTexture();
	}

	CreateUniformBufferObject();
	
	CreateDescriptorPool();
	CreateDescriptorSet();

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

void VkGameObject::CreateVertexBufferObject()
{
	std::vector<Vertex> vertices = meshRenderer->GetMesh()->vertices;

	BufferObject stagingBufferObject;
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	stagingBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = sizeof(vertices[0]) * vertices.size();

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBufferObject);

	void* data;
	vkMapMemory(createInfo.logicalDevice, stagingBufferObject.GetVkBufferMemory(), 0, stagingBufferCreateInfo.bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)stagingBufferCreateInfo.bufferSize);
	vkUnmapMemory(createInfo.logicalDevice, stagingBufferObject.GetVkBufferMemory());

	BufferObjectVkCreateInfo vertexBuffeCreateInfo;
	vertexBuffeCreateInfo.physicalDevice = createInfo.physicalDevice;
	vertexBuffeCreateInfo.logicalDevice = createInfo.logicalDevice;
	vertexBuffeCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	vertexBuffeCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	vertexBuffeCreateInfo.bufferSize = stagingBufferCreateInfo.bufferSize;

	BufferObject::InitializeBufferObject(vertexBuffeCreateInfo, &vertexBufferObject);

	stagingBufferObject.CopyBuffer(&vertexBufferObject, createInfo.commandPool, createInfo.graphicsQueue, stagingBufferCreateInfo.bufferSize);
	stagingBufferObject.Cleanup();
}

void VkGameObject::CreateIndexBufferObject()
{
	std::vector<uint16_t> indices = meshRenderer->GetMesh()->indices;

	BufferObject stagingBufferObject;
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	stagingBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = sizeof(indices[0]) * indices.size();

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBufferObject);

	void* data;
	vkMapMemory(createInfo.logicalDevice, stagingBufferObject.GetVkBufferMemory(), 0, stagingBufferCreateInfo.bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)stagingBufferCreateInfo.bufferSize);
	vkUnmapMemory(createInfo.logicalDevice, stagingBufferObject.GetVkBufferMemory());

	BufferObjectVkCreateInfo indexBufferCreateInfo;
	indexBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	indexBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	indexBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	indexBufferCreateInfo.bufferSize = stagingBufferCreateInfo.bufferSize;

	BufferObject::InitializeBufferObject(indexBufferCreateInfo, &indexBufferObject);

	stagingBufferObject.CopyBuffer(&indexBufferObject, createInfo.commandPool, createInfo.graphicsQueue, stagingBufferCreateInfo.bufferSize);
	stagingBufferObject.Cleanup();
}

void VkGameObject::CreateVkTexture()
{
	VkTextureVkCreateInfo textCreateInfo{};
	textCreateInfo.logicalDevice = createInfo.logicalDevice;
	textCreateInfo.physicalDevice = createInfo.physicalDevice;
	textCreateInfo.graphicsQueue = createInfo.graphicsQueue;
	textCreateInfo.commandPool = createInfo.commandPool;
	textCreateInfo.texture = meshRenderer->GetTexture();
	textCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	textCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	textCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	textCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	VkTexture::InitializeVkTexture(textCreateInfo, &vkTexture);
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

void VkGameObject::CreateDescriptorPool()
{
	DescriptorPoolVkCreateInfo poolCreateInfo{};
	poolCreateInfo.logicalDevice = createInfo.logicalDevice;
	poolCreateInfo.frameCount = MAX_FRAMES_IN_FLIGHT;

	DescriptorPool::InitializeDescriptorPool(poolCreateInfo, &descriptorPool);
}

void VkGameObject::CreateDescriptorSet()
{
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, graphicsPipeline.GetDescriptorSetLayout().GetDescriptorSetLayout());
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool.GetDescriptorPool();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(createInfo.logicalDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBufferObject[i].GetVkBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferData);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = vkTexture.GetImageView();
		imageInfo.sampler = vkTexture.GetSampler();

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

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

		vkUpdateDescriptorSets(createInfo.logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void VkGameObject::Draw(VkCommandBuffer _commandBuffer, int _currentFrame) const
{
	if (HasMeshRenderer())
	{
		vkCmdBindPipeline(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetGraphicsPipeline());

		VkBuffer vertexBuffers[] = { vertexBufferObject.GetVkBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(_commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(_commandBuffer, indexBufferObject.GetVkBuffer(), 0, VK_INDEX_TYPE_UINT16);

		vkCmdBindDescriptorSets(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetGraphicsPipelineLayout(), 0, 1, &descriptorSets[_currentFrame], 0, nullptr);

		vkCmdDrawIndexed(_commandBuffer, static_cast<uint32_t>(meshRenderer->GetMesh()->indices.size()), 1, 0, 0, 0);
	}
}

MeshRenderer* VkGameObject::GetMeshRenderer() const
{
	return meshRenderer;
}

const BufferObject& VkGameObject::GetVBO() const
{
	return vertexBufferObject;
}

const BufferObject& VkGameObject::GetIBO() const
{
	return indexBufferObject;
}

bool VkGameObject::HasMeshRenderer() const
{
	return meshRenderer != nullptr;
}

void VkGameObject::Update(size_t _currentframe)
{
	UniformBufferData uboData{};

	uboData.model = createInfo.gameObject->GetWorldTransform().ToMatrixWithScale().Transpose();
	uboData.view = Mathlib::Mat4::ViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.0f, 0.0f, -15.0f), Mathlib::Vec3(0.0f, 0.0f, 0.0f), Mathlib::Vec3(0.0f, -1.0f, 0.0f)).Transpose();
	uboData.proj = Mathlib::Mat4::PerspectiveMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Math::Radians(45.0f), 1024.f / 720.f, 0.1f, 100.0f).Transpose();

	uniformBufferObject.CopyDataToBuffer<UniformBufferData>(_currentframe, &uboData, sizeof(UniformBufferData));
}

void VkGameObject::Cleanup()
{
	uniformBufferObject.Cleanup();
	descriptorPool.Cleanup();

	if (meshRenderer != nullptr)
	{
		vkTexture.Cleanup();
		indexBufferObject.Cleanup();
		vertexBufferObject.Cleanup();
	}

	graphicsPipeline.Cleanup();
}