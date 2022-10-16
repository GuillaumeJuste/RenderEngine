#include "Engine/Vulkan/Descriptor/Set/DescriptorSet.hpp"

#include <stdexcept>
#include <array>

using namespace RenderEngine::Engine::Vulkan;

void DescriptorSet::InitializeDescriptorSet(const DescriptorSetVkCreateInfo& _createInfo, DescriptorSet* _output)
{
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->descriptorSetLayout = _createInfo.descriptorSetLayout;
	_output->descriptorPool = _createInfo.descriptorPool;

	std::vector<VkDescriptorSetLayout> layouts(_createInfo.frameCount, _output->descriptorSetLayout.GetDescriptorSetLayout());
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _output->descriptorPool->GetDescriptorPool();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(_createInfo.frameCount);
	allocInfo.pSetLayouts = layouts.data();

	_output->descriptorSets.resize(_createInfo.frameCount);
	if (vkAllocateDescriptorSets(_output->logicalDevice, &allocInfo, _output->descriptorSets.data()) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < _createInfo.frameCount; i++) 
	{
		const size_t bufferSize = _createInfo.descriptorSetBufferDatas[i].descriptorBufferInfos.size();
		const size_t imageSize = _createInfo.descriptorSetImageDatas[i].descriptorImageInfos.size();

		std::vector<VkWriteDescriptorSet> descriptorWrites{};

		descriptorWrites.resize(bufferSize + imageSize);

		for (int j = 0; j < bufferSize; j++)
		{
			descriptorWrites[j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[j].dstSet = _output->descriptorSets[i];
			descriptorWrites[j].dstBinding = j;
			descriptorWrites[j].dstArrayElement = 0;
			descriptorWrites[j].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[j].descriptorCount = 1;
			descriptorWrites[j].pBufferInfo = &_createInfo.descriptorSetBufferDatas[i].descriptorBufferInfos[j];
		}

		for (int j = 0; j < imageSize; j++)
		{
			descriptorWrites[bufferSize + j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[bufferSize + j].dstSet = _output->descriptorSets[i];
			descriptorWrites[bufferSize + j].dstBinding = bufferSize + j;
			descriptorWrites[bufferSize + j].dstArrayElement = 0;
			descriptorWrites[bufferSize + j].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[bufferSize + j].descriptorCount = 1;
			descriptorWrites[bufferSize + j].pImageInfo = &_createInfo.descriptorSetImageDatas[i].descriptorImageInfos[j];
		}
		
		vkUpdateDescriptorSets(_output->logicalDevice, static_cast<uint32_t>(bufferSize + imageSize), descriptorWrites.data(), 0, nullptr);
	}
}

const VkDescriptorSet& DescriptorSet::GetFrameDescriptorSet(int _frame) const
{
	return descriptorSets[_frame];
}

void DescriptorSet::Cleanup()
{

}