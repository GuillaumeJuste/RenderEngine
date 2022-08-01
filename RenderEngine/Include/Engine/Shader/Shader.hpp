#pragma once

#ifndef RENDERENGINE_SHADER
#define RENDERENGINE_SHADER

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Shader/ShaderCreateInfo.hpp"

namespace RenderEngine::Vulkan
{
	class Shader
	{
	private:
		VkDevice logicalDevice;
		VkShaderModule shaderModule;
		ShaderType shaderType;
		VkPipelineShaderStageCreateInfo shaderStageInfo;

		static std::vector<char> ReadShaderFile(const std::string& _shaderFilePath);

	public:
		Shader() = default;
		~Shader() = default;

		static void CreateShader(ShaderCreateInfo _createInfo, Shader* _output);
		void Cleanup();

		const VkShaderModule& GetShaderModule() const;
		const VkPipelineShaderStageCreateInfo& GetShaderStageInfo() const;
	};

}

#endif
