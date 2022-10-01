#pragma once

#ifndef RENDERENGINE_UNIFORMBUFFEROBJECT
#define RENDERENGINE_UNIFORMBUFFEROBJECT

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/UniformBuffer/UniformBufferObjectVkCreateInfo.hpp"
#include "Engine/Vulkan/BufferObject/BufferObject.hpp"

namespace RenderEngine::Engine::Vulkan
{
	/**
	 * @brief Class to manage creation and usage of Vulkan ImageView
	*/
	class UniformBufferObject
	{
	private:
		VkDevice logicalDevice = VK_NULL_HANDLE;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		UniformBufferData uniformBufferData;

		BufferObject uniformBuffer;

	public:

		/// default constructor
		UniformBufferObject() = default;

		/// default destructor
		~UniformBufferObject() = default;

		static void InitializeUniformBufferObject(const UniformBufferObjectVkCreateInfo& _createInfo, UniformBufferObject* _output);

		void UpdateUniformBufferObject(const UniformBufferData& _newData);

		const BufferObject& GetBufferObject() const;

		void Cleanup();
	};

}

#endif