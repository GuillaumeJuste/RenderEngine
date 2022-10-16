#include "Engine/Vulkan/Descriptor/Layout/DescriptorSetLayout.hpp"

#include <stdexcept>
#include <array>

using namespace RenderEngine::Engine::Vulkan;

void DescriptorSetLayout::InitializeDescriptorSetLayout(const DescriptorSetLayoutVkCreateInfo& _createInfo, DescriptorSetLayout* _output)
{
    _output->logicalDevice = _createInfo.logicalDevice;

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

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