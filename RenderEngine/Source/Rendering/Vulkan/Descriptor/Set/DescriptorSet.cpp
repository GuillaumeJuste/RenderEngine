#include "Rendering/Vulkan/Descriptor/Set/DescriptorSet.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorData.hpp"

#include <stdexcept>
#include <list>
#include <vector>

using namespace RenderEngine::Rendering;

void DescriptorSet::InitializeDescriptorSet(const DescriptorSetVkCreateInfo& _createInfo, DescriptorSet* _output)
{
	_output->logicalDevice = _createInfo.logicalDevice;

	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, _createInfo.descriptorSetLayout.GetDescriptorSetLayout());
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _createInfo.descriptorPool.GetVkDescriptorPool();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	_output->descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(_createInfo.logicalDevice, &allocInfo, _output->descriptorSets.data()) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	size_t descriptorCount = _createInfo.descriptorDatas.Size();
	for (size_t frameIndex = 0; frameIndex < _createInfo.frameCount; frameIndex++)
	{
		std::list<VkDescriptorBufferInfo>  DBI{};
		std::list<VkDescriptorImageInfo>  DII{};
		std::vector<VkWriteDescriptorSet> descriptorWrites{};

		for (int descriptorIndex = 0; descriptorIndex < descriptorCount; descriptorIndex++)
		{
			DescriptorData data = _createInfo.descriptorDatas[descriptorIndex];

			if (data.buffer != nullptr && (data.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER || data.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER))
			{
				VkDescriptorBufferInfo* bufferInfo = &DBI.emplace_back();
				bufferInfo->buffer = data.buffer->operator[](frameIndex).GetVkBuffer();
				bufferInfo->offset = 0;
				bufferInfo->range = data.buffer->operator[](frameIndex).GetBufferSize();

				VkWriteDescriptorSet descriptorWrite{};
				descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrite.dstSet = _output->descriptorSets[frameIndex];
				descriptorWrite.dstBinding = data.binding;
				descriptorWrite.dstArrayElement = 0;
				descriptorWrite.descriptorType = data.descriptorType;
				descriptorWrite.descriptorCount = 1;
				descriptorWrite.pBufferInfo = bufferInfo;

				descriptorWrites.push_back(descriptorWrite);
			}

			else if (data.texture != nullptr && data.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
			{
				VkDescriptorImageInfo* imageInfo = &DII.emplace_back();
				imageInfo->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo->imageView = data.texture->GetImageView();
				imageInfo->sampler = data.texture->GetSampler();

				VkWriteDescriptorSet descriptorWrite{};
				descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrite.dstSet = _output->descriptorSets[frameIndex];
				descriptorWrite.dstBinding = data.binding;
				descriptorWrite.dstArrayElement = 0;
				descriptorWrite.descriptorType = data.descriptorType;
				descriptorWrite.descriptorCount = 1;
				descriptorWrite.pImageInfo = imageInfo;

				descriptorWrites.push_back(descriptorWrite);
			}
		}

		vkUpdateDescriptorSets(_createInfo.logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

const VkDescriptorSet& DescriptorSet::GetFrameDescriptorSet(int _frame) const
{
	return descriptorSets[_frame];
}

void DescriptorSet::Cleanup()
{

}