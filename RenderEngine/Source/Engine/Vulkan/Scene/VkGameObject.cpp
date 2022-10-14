#include "Engine/Vulkan/Scene/VkGameObject.hpp"

#include "Engine/Vulkan/UniformBuffer/UniformBufferData.hpp"

#include "Misc/Math.hpp"

using namespace RenderEngine::Engine::Vulkan;

VkGameObject::VkGameObject(const VkGameObjectCreateInfo& _createInfo) :
	createInfo{ _createInfo }
{
	meshRenderer = createInfo.gameObject->GetComponent<MeshRenderer>();
	if (meshRenderer != nullptr)
	{
		CreateVertexBufferObject();
		CreateIndexBufferObject();
	}

	CreateUniformBufferObject();
	
	CreateDescriptorPool();
	CreateDescriptorSet();

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
	DescriptorSetVkCreateInfo descriptorSetCreateInfo{};

	descriptorSetCreateInfo.logicalDevice = createInfo.logicalDevice;
	descriptorSetCreateInfo.descriptorSetLayout = createInfo.graphicsPipeline->GetDescriptorSetLayout();
	descriptorSetCreateInfo.descriptorPool = &descriptorPool;
	descriptorSetCreateInfo.frameCount = MAX_FRAMES_IN_FLIGHT;
	descriptorSetCreateInfo.descriptorSetDatas.resize(MAX_FRAMES_IN_FLIGHT);

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBufferObject[i].GetVkBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferData);

		descriptorSetCreateInfo.descriptorSetDatas[i].descriptorBufferInfos.push_back(bufferInfo);
	}

	DescriptorSet::InitializeDescriptorSet(descriptorSetCreateInfo, &descriptorSet);
}

void VkGameObject::Draw(VkCommandBuffer _commandBuffer, int _currentFrame) const
{
	if (HasMeshRenderer())
	{
		VkBuffer vertexBuffers[] = { vertexBufferObject.GetVkBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(_commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(_commandBuffer, indexBufferObject.GetVkBuffer(), 0, VK_INDEX_TYPE_UINT16);

		vkCmdBindDescriptorSets(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, createInfo.graphicsPipeline->GetGraphicsPipelineLayout(), 0, 1, &descriptorSet.GetFrameDescriptorSet(_currentFrame), 0, nullptr);

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
	descriptorSet.Cleanup();
	descriptorPool.Cleanup();

	indexBufferObject.Cleanup();
	vertexBufferObject.Cleanup();
}