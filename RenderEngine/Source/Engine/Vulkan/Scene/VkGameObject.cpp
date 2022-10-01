#include "Engine/Vulkan/Scene/VkGameObject.hpp"
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
	
	CreateDescriptorSet();

}

void VkGameObject::CreateVertexBufferObject()
{
	std::vector<Vertex> vertices = meshRenderer->GetMesh()->GetVertices();

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
	std::vector<uint16_t> indices = meshRenderer->GetMesh()->GetIndices();

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
	UniformBufferObjectVkCreateInfo uboCreateInfo{};
	uboCreateInfo.physicalDevice = createInfo.physicalDevice;
	uboCreateInfo.logicalDevice = createInfo.logicalDevice;

	UniformBufferData uboData{};

	uboData.model = createInfo.gameObject->GetWorldTransform().ToMatrixWithScale().Transpose();
	uboData.view = Mathlib::Mat4::ViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.0f, 0.0f, -10.0f), Mathlib::Vec3(0.0f, 0.0f, 0.0f), Mathlib::Vec3(0.0f, -1.0f, 0.0f)).Transpose();
	uboData.proj = Mathlib::Mat4::PerspectiveMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Math::Radians(45.0f), 1024.f / 720.f, 0.1f, 100.0f).Transpose();

	uboCreateInfo.uniformBufferData = uboData;

	uniformBufferObjects.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		UniformBufferObject::InitializeUniformBufferObject(uboCreateInfo, &uniformBufferObjects[i]);
	}
}

void VkGameObject::CreateDescriptorSet()
{
	DescriptorSetVkCreateInfo descriptorSetCreateInfo{};

	descriptorSetCreateInfo.logicalDevice = createInfo.logicalDevice;
	descriptorSetCreateInfo.descriptorSetLayout = createInfo.descriptorSetLayout;
	descriptorSetCreateInfo.descriptorPool = createInfo.descriptorPool;
	descriptorSetCreateInfo.frameCount = MAX_FRAMES_IN_FLIGHT;
	descriptorSetCreateInfo.descriptorSetDatas.resize(MAX_FRAMES_IN_FLIGHT);

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBufferObjects[i].GetBufferObject().GetVkBuffer();
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

		vkCmdDrawIndexed(_commandBuffer, static_cast<uint32_t>(meshRenderer->GetMesh()->GetIndices().size()), 1, 0, 0, 0);
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

const BufferObject& VkGameObject::GetUBO(uint32_t _frameIndex) const
{
	return uniformBufferObjects[_frameIndex].GetBufferObject();
}

bool VkGameObject::HasMeshRenderer() const
{
	return meshRenderer != nullptr;
}

void VkGameObject::Cleanup()
{
	for (std::vector<UniformBufferObject>::iterator it = uniformBufferObjects.begin(); it != uniformBufferObjects.end(); ++it)
		it->Cleanup();
	indexBufferObject.Cleanup();
	vertexBufferObject.Cleanup();
}