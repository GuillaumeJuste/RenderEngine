#include "Vulkan/Shader/ShaderCreateInfo.hpp"

using namespace RenderEngine::Vulkan;

ShaderCreateInfo::ShaderCreateInfo(VkShaderStageFlagBits _shaderType, std::string _shaderFilePath,const VkDevice& _device) :
	shaderType{ _shaderType }, shaderFilePath{ _shaderFilePath }, device { _device }
{
}