namespace RenderEngine::Rendering
{
	template<typename T>
	void DescriptorBuffer::CopyDataToBuffer(int _bufferIndex, T* _data, size_t _offset, size_t _dataSize)
	{
		if (_dataSize <= 0)
			return;

		void* data;
		vkMapMemory(logicalDevice, bufferObjects[_bufferIndex].GetVkBufferMemory(), _offset, _dataSize, 0, &data);
		memcpy(data, _data, _dataSize);
		vkUnmapMemory(logicalDevice, bufferObjects[_bufferIndex].GetVkBufferMemory());
	}
}

	