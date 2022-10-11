namespace RenderEngine::Engine::Vulkan
{
	template<typename T>
	void DescriptorBuffer::CopyDataToBuffer(int _bufferIndex, T* _data, size_t _dataSize)
	{
		void* data;
		vkMapMemory(logicalDevice, bufferObjects[_bufferIndex].GetVkBufferMemory(), 0, _dataSize, 0, &data);
		memcpy(data, _data, _dataSize);
		vkUnmapMemory(logicalDevice, bufferObjects[_bufferIndex].GetVkBufferMemory());
	}
}

	