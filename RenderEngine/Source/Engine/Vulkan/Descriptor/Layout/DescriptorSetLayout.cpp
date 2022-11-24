#include "Engine/Vulkan/Descriptor/Layout/DescriptorSetLayout.hpp"

#include <stdexcept>
#include <array>

using namespace RenderEngine::Engine::Vulkan;

void DescriptorSetLayout::InitializeDescriptorSetLayout(const DescriptorSetLayoutVkCreateInfo& _createInfo, DescriptorSetLayout* _output)
{
    _output->logicalDevice = _createInfo.logicalDevice;

    size_t descriptorCount = _createInfo.descriptorSetDatas.size();

    std::vector<VkDescriptorSetLayoutBinding> bindings{};

    for (size_t i = 0; i < descriptorCount; i++)
    {
        VkDescriptorSetLayoutBinding setLayout{};

        setLayout.binding = _createInfo.descriptorSetDatas[i].binding;
        setLayout.descriptorType = _createInfo.descriptorSetDatas[i].descriptorType;
        setLayout.descriptorCount = 1;
        setLayout.stageFlags = _createInfo.descriptorSetDatas[i].stageFlags;
        setLayout.pImmutableSamplers = nullptr;

        bindings.push_back(setLayout);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(_output->logicalDevice, &layoutInfo, nullptr, &_output->descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

}

const VkDescriptorSetLayout& DescriptorSetLayout::GetDescriptorSetLayout() const
{
    return descriptorSetLayout;
}

void DescriptorSetLayout::Cleanup()
{
    vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
}