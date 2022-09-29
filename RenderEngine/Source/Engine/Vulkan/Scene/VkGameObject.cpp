#include "Engine/Vulkan/Scene/VkGameObject.hpp"

using namespace RenderEngine::Engine::Vulkan;

VkGameObject::VkGameObject(const VkGameObjectCreateInfo& _createInfo) :
	createInfo { _createInfo }
{
	meshRenderer = createInfo.gameObject->GetComponent<MeshRenderer>();
	if (meshRenderer != nullptr)
	{
		CreateVertexBufferObject();
		CreateIndexBufferObject();
	}
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
	std::vector<uint32_t> indices = meshRenderer->GetMesh()->GetIndices();

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

void VkGameObject::Draw(VkCommandBuffer _commandBuffer)
{
	if (HasMeshRenderer())
	{
		VkBuffer vertexBuffers[] = { vertexBufferObject.GetVkBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(_commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(_commandBuffer, indexBufferObject.GetVkBuffer(), 0, VK_INDEX_TYPE_UINT16);

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

bool VkGameObject::HasMeshRenderer() const
{
	return meshRenderer != nullptr;
}

void VkGameObject::Cleanup()
{
	indexBufferObject.Cleanup();
	vertexBufferObject.Cleanup();
}