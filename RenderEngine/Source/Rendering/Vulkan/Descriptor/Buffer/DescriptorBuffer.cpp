#include "Rendering/Vulkan/Descriptor/Buffer/DescriptorBuffer.hpp"

using namespace RenderEngine::Rendering;

void DescriptorBuffer::InitializeDescriptorBuffer(const BufferObjectVkCreateInfo& _createInfo, int _frameCount, DescriptorBuffer* _output)
{
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->bufferObjects.resize(_frameCount);
	_output->bufferSize = _createInfo.bufferSize;

	for (size_t i = 0; i < _frameCount; i++)
	{
		BufferObject::InitializeBufferObject(_createInfo, &_output->bufferObjects[i]);
	}
}

void DescriptorBuffer::Cleanup()
{
	for (std::vector<BufferObject>::iterator it = bufferObjects.begin(); it != bufferObjects.end(); ++it)
		it->Clean();
}

const BufferObject& DescriptorBuffer::operator[](size_t _index)
{
	return bufferObjects[_index];
}

const size_t& DescriptorBuffer::GetBufferSize() const
{
	return bufferSize;
}