#include "Rendering/Vulkan/Shader/ShaderVkCreateInfo.hpp"

using namespace RenderEngine::Rendering;

ShaderVkCreateInfo::ShaderVkCreateInfo(VkShaderStageFlagBits _shaderType, std::string _shaderFilePath,const VkDevice& _device) :
	shaderType{ _shaderType }, shaderFilePath{ _shaderFilePath }, device { _device }
{
}