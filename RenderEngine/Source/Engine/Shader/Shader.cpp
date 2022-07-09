#include "Engine/Shader/Shader.hpp"

using namespace RenderEngine;

Shader Shader::CreateShader(ShaderCreateInfo _createInfo)
{
    auto shaderCode = readFile(_createInfo.shaderFilePath);

    Shader newShader;
    newShader.logicalDevice = _createInfo.device;
    newShader.shaderType = _createInfo.shaderType;

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

    if (vkCreateShaderModule(_createInfo.device, &createInfo, nullptr, &(newShader.shaderModule)) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create shader module!");
    }

    newShader.shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    newShader.shaderStageInfo.stage = static_cast<VkShaderStageFlagBits>(newShader.shaderType);
    newShader.shaderStageInfo.module = newShader.shaderModule;
    newShader.shaderStageInfo.pName = _createInfo.shaderFilePath.c_str();

    return newShader;
}


std::vector<char> readFile(const std::string& _shaderFilePath)
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
}

const VkShaderModule& Shader::GetShaderModule() const
{
    return shaderModule;
}
const VkPipelineShaderStageCreateInfo& Shader::GetShaderStageInfo() const
{
    return shaderStageInfo;
}