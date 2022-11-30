#pragma once

#ifndef RENDERENGINE_DESCRIPTORBUFFER
#define RENDERENGINE_DESCRIPTORBUFFER

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/BufferObject/BufferObject.hpp"

#include <vector>

namespace RenderEngine::Engine::Vulkan
{
	class DescriptorBuffer
	{
	private:
		VkDevice logicalDevice = VK_NULL_HANDLE;
	
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
	};

}

#include "Engine/Vulkan/Descriptor/Buffer/DescriptorBuffer.inl"

#endif