#include "Engine/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"

using namespace RenderEngine::Engine::Vulkan;

void DescriptorBuffer::InitializeDescriptorBuffer(const BufferObjectVkCreateInfo& _createInfo, int _frameCount, DescriptorBuffer* _output)
{
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->bufferObjects.resize(_frameCount);

	for (size_t i = 0; i < _frameCount; i++)
	{
		BufferObject::InitializeBufferObject(_createInfo, &_output->bufferObjects[i]);
	}
}

void DescriptorBuffer::Cleanup()
{
	for (std::vector<BufferObject>::iterator it = bufferObjects.begin(); it != bufferObjects.end(); ++it)
		it->Cleanup();
}

const BufferObject& DescriptorBuffer::operator[](int _index)
{
	return bufferObjects[_index];
}