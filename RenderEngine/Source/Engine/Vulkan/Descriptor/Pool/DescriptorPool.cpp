#include "Engine/Vulkan/Descriptor/Pool/DescriptorPool.hpp"
#include <stdexcept>

using namespace RenderEngine::Engine::Vulkan;

void DescriptorPool::InitializeDescriptorPool(const DescriptorPoolVkCreateInfo& _createInfo, DescriptorPool* _output)
{
	_output->logicalDevice = _createInfo.logicalDevice;

	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * 2);

	if (vkCreateDescriptorPool(_output->logicalDevice, &poolInfo, nullptr, &_output->descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

const VkDescriptorPool& DescriptorPool::GetDescriptorPool() const
{
	return descriptorPool;
}

void DescriptorPool::Cleanup()
{
	vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
}