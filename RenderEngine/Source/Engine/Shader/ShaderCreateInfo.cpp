#include "Engine/Shader/ShaderCreateInfo.hpp"

using namespace RenderEngine;

ShaderCreateInfo::ShaderCreateInfo(ShaderType _shaderType, std::string _shaderFilePath, const VkDevice& _device) :
	shaderType{ _shaderType }, shaderFilePath{ _shaderFilePath }, device { _device }
{
}