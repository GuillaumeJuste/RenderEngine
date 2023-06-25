namespace RenderEngine::Rendering
{
	template<typename T>
	bool VkTexture::FillImageBuffer(T* _imageData)
	{
		BufferObject stagingBuffer;
		BufferObjectVkCreateInfo stagingBufferCreateInfo;
		stagingBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
		stagingBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
		stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		stagingBufferCreateInfo.bufferSize = createInfo.texture.imageSize * createInfo.texture.imageCount;

		BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBuffer);

		void* data;
		vkMapMemory(createInfo.logicalDevice, stagingBuffer.GetVkBufferMemory(), 0, stagingBuffer.GetBufferSize(), 0, &data);
		memcpy(data, _imageData, stagingBuffer.GetBufferSize());
		vkUnmapMemory(createInfo.logicalDevice, stagingBuffer.GetVkBufferMemory());

		image.TransitionImageLayout(createInfo.format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		image.CopyBufferToImage(stagingBuffer.GetVkBuffer());

		if (createInfo.texture.mipLevels > 1)
			image.GenerateMipmaps(createInfo.format);
		else
			image.TransitionImageLayout(createInfo.format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		stagingBuffer.Clean();


		return false;
	}
}