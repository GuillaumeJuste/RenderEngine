#include "Rendering/Vulkan/Shader/VkShader.hpp"
#include <stdexcept>

using namespace RenderEngine::Rendering;

void VkShader::CreateVkShader(VkShaderCreateInfo _createInfo, VkShader* _output)
{
    _output->logicalDevice = _createInfo.device;
    _output->shaderType = EnumToVkFlag(_createInfo.rawShader.stage);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = _createInfo.rawShader.shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(_createInfo.rawShader.shaderCode.data());

    if (vkCreateShaderModule(_createInfo.device, &createInfo, nullptr, &(_output->shaderModule)) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create shader module!");
    }

    _output->shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    _output->shaderStageInfo.stage = _output->shaderType;
    _output->shaderStageInfo.module = _output->shaderModule;
    _output->shaderStageInfo.pName = "main";
    _output->shaderStageInfo.pNext = VK_NULL_HANDLE;
    _output->shaderStageInfo.pSpecializationInfo = VK_NULL_HANDLE;
    _output->shaderStageInfo.flags = 0;

}

VkShaderStageFlagBits VkShader::EnumToVkFlag(RenderEngine::Assets::SHADER_STAGE _stage)
{
    switch (_stage)
    {
    case RenderEngine::Assets::VERTEX:
        return VK_SHADER_STAGE_VERTEX_BIT;
    case RenderEngine::Assets::FRAGMENT:
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    case RenderEngine::Assets::COMPUTE:
        return VK_SHADER_STAGE_COMPUTE_BIT;
    case RenderEngine::Assets::GEOMETRY:
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    }
    return VK_SHADER_STAGE_VERTEX_BIT;
}

void VkShader::Clean()
{
    vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
}

const VkShaderModule& VkShader::GetShaderModule() const
{
    return shaderModule;
}
const VkPipelineShaderStageCreateInfo& VkShader::GetShaderStageInfo() const
{
    return shaderStageInfo;
}