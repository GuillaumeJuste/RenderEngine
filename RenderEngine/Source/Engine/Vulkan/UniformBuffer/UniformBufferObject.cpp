#include "Engine/Vulkan/UniformBuffer/UniformBufferObject.hpp"

using namespace RenderEngine::Engine::Vulkan;

void UniformBufferObject::InitializeUniformBufferObject(const UniformBufferObjectVkCreateInfo& _createInfo, UniformBufferObject* _output)
{
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->physicalDevice = _createInfo.physicalDevice;
	_output->uniformBufferData = _createInfo.uniformBufferData;

	BufferObjectVkCreateInfo uniformBufferCreateInfo;
	uniformBufferCreateInfo.physicalDevice = _output->physicalDevice;
	uniformBufferCreateInfo.logicalDevice = _output->logicalDevice;
	uniformBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	uniformBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	uniformBufferCreateInfo.bufferSize = sizeof(UniformBufferData);

	BufferObject::InitializeBufferObject(uniformBufferCreateInfo, &_output->uniformBuffer);

	_output->UpdateUniformBufferObject(_output->uniformBufferData);
}

void UniformBufferObject::UpdateUniformBufferObject(const UniformBufferData& _newData)
{
	uniformBufferData = _newData;

	void* data;
	vkMapMemory(logicalDevice, uniformBuffer.GetVkBufferMemory(), 0, sizeof(UniformBufferData), 0, &data);
	memcpy(data, &uniformBufferData, sizeof(UniformBufferData));
	vkUnmapMemory(logicalDevice, uniformBuffer.GetVkBufferMemory());
}

const BufferObject& UniformBufferObject::GetBufferObject() const
{
	return uniformBuffer;
}

void UniformBufferObject::Cleanup()
{
	uniformBuffer.Cleanup();
}