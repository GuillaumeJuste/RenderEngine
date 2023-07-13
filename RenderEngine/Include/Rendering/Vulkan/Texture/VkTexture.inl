namespace RenderEngine::Rendering
{
	template<typename T>
	bool VkTexture::FillImageBuffer(T* _imageData, uint32_t _totalImageSize, bool _copyMipMap)
	{
		BufferObject stagingBuffer;
		BufferObjectVkCreateInfo stagingBufferCreateInfo;
		stagingBufferCreateInfo.physicalDevice = physicalDevice;
		stagingBufferCreateInfo.logicalDevice = logicalDevice;
		stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		stagingBufferCreateInfo.bufferSize = _totalImageSize;

		BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBuffer);

		void* data;
		vkMapMemory(logicalDevice, stagingBuffer.GetVkBufferMemory(), 0, stagingBuffer.GetBufferSize(), 0, &data);
		memcpy(data, _imageData, stagingBuffer.GetBufferSize());
		vkUnmapMemory(logicalDevice, stagingBuffer.GetVkBufferMemory());

		VkCommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(logicalDevice, commandPool);
		imageBuffer.TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, commandBuffer);
		imageBuffer.CopyBufferToImage(stagingBuffer.GetVkBuffer(), elementSize, commandBuffer, _copyMipMap);
		CommandBuffer::EndSingleTimeCommands(logicalDevice, commandPool, graphicsQueue, commandBuffer);

		stagingBuffer.Clean();

		return true;
	}
}