#include "Engine/Shader/Shader.hpp"

#include <iostream>

using namespace RenderEngine::Vulkan;

void Shader::CreateShader(ShaderCreateInfo _createInfo, Shader* _output)
{
    auto shaderCode = ReadShaderFile(_createInfo.shaderFilePath);

    _output->logicalDevice = _createInfo.device;
    _output->shaderType = _createInfo.shaderType;

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

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


std::vector<char> Shader::ReadShaderFile(const std::string& _shaderFilePath)
{
    std::ifstream file(_shaderFilePath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file at : " + _shaderFilePath);
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

void Shader::Cleanup()
{
    vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
    std::cout << "[Cleaned] Shader" << std::endl;
}

const VkShaderModule& Shader::GetShaderModule() const
{
    return shaderModule;
}
const VkPipelineShaderStageCreateInfo& Shader::GetShaderStageInfo() const
{
    return shaderStageInfo;
}