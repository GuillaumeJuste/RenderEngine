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

    VkDescriptorSetLayoutBinding cameraLayoutBinding{};
    cameraLayoutBinding.binding = 1;
    cameraLayoutBinding.descriptorCount = 1;
    cameraLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    cameraLayoutBinding.pImmutableSamplers = nullptr;
    cameraLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 2;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding materialBinding{};
    materialBinding.binding = 3;
    materialBinding.descriptorCount = 1;
    materialBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    materialBinding.pImmutableSamplers = nullptr;
    materialBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding pointLightLayoutBinding{};
    pointLightLayoutBinding.binding = 4;
    pointLightLayoutBinding.descriptorCount = 1;
    pointLightLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    pointLightLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding specularSamplerLayoutBinding{};
    specularSamplerLayoutBinding.binding = 5;
    specularSamplerLayoutBinding.descriptorCount = 1;
    specularSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    specularSamplerLayoutBinding.pImmutableSamplers = nullptr;
    specularSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding directionalLightLayoutBinding{};
    directionalLightLayoutBinding.binding = 6;
    directionalLightLayoutBinding.descriptorCount = 1;
    directionalLightLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    directionalLightLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding spotLightLayoutBinding{};
    spotLightLayoutBinding.binding = 7;
    spotLightLayoutBinding.descriptorCount = 1;
    spotLightLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    spotLightLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 8> bindings = { uboLayoutBinding, samplerLayoutBinding, cameraLayoutBinding, materialBinding, pointLightLayoutBinding, specularSamplerLayoutBinding, directionalLightLayoutBinding, spotLightLayoutBinding };

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