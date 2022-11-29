#include "Engine/Vulkan/Descriptor/Pool/DescriptorPool.hpp"
#include <stdexcept>
#include <array>

using namespace RenderEngine::Engine::Vulkan;

void DescriptorPool::InitializeDescriptorPool(const DescriptorPoolVkCreateInfo& _createInfo, DescriptorPool* _output)
{
	_output->logicalDevice = _createInfo.logicalDevice;

	size_t descriptorCount = _createInfo.descriptorDatas.Size();

	std::vector<VkDescriptorPoolSize> poolSizes{};

	for (size_t i = 0; i < descriptorCount; i++)
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.descriptorCount = _createInfo.frameCount;
		poolSize.type = _createInfo.descriptorDatas[i].descriptorType;

		poolSizes.push_back(poolSize);
	}

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(_output->logicalDevice, &poolInfo, nullptr, &_output->descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

const VkDescriptorPool& DescriptorPool::GetVkDescriptorPool() const
{
	return descriptorPool;
}

void DescriptorPool::Cleanup()
{
	vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
}