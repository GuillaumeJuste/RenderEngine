#pragma once

#ifndef RENDERENGINE_DESCRIPTORBUFFER
#define RENDERENGINE_DESCRIPTORBUFFER

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/BufferObject/BufferObject.hpp"

#include <vector>

namespace RenderEngine::Rendering
{
	class DescriptorBuffer
	{
	private:
		VkDevice logicalDevice = VK_NULL_HANDLE;
		
		size_t bufferSize = 0;

		std::vector<BufferObject> bufferObjects;
	public:

		/// default constructor
		DescriptorBuffer() = default;

		/// default destructor
		~DescriptorBuffer() = default;

		static void InitializeDescriptorBuffer(const BufferObjectVkCreateInfo& _createInfo, int _frameCount, DescriptorBuffer* _output);

		template<typename T>
		void CopyDataToBuffer(int _bufferIndex, T* _data, size_t _dataSize);

		void Cleanup();

		const BufferObject& operator[](size_t _index);

		const size_t& GetBufferSize() const;
	};

}

#include "Rendering/Vulkan/Descriptor/Buffer/DescriptorBuffer.inl"

#endif