#pragma once

#ifndef RENDERENGINE_SHADER
#define RENDERENGINE_SHADER

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/Shader/ShaderCreateInfo.hpp"

namespace RenderEngine::Vulkan
{
	/**
	 * @brief Class to manage creation and usage of a shader
	*/
	class Shader
	{
	private:

		/// logical device
		VkDevice logicalDevice;

		/// shader module
		VkShaderModule shaderModule;

		/// shader type
		VkShaderStageFlagBits shaderType;
		
		/// shader stage info
		VkPipelineShaderStageCreateInfo shaderStageInfo;

		/**
		 * @brief Read shader file path
		 * @param _shaderFilePath file path
		 * @return shader content
		*/
		static std::vector<char> ReadShaderFile(const std::string& _shaderFilePath);

	public:
		/// default constructor
		Shader() = default;

		/// default destructor
		~Shader() = default;

		/**
		 * @brief Create shader
		 * @param _createInfo shader create info
		 * @param _output shader to initilize
		*/
		static void CreateShader(ShaderCreateInfo _createInfo, Shader* _output);
		
		/**
		 * @brief Clean up vulkan classes
		*/
		void Cleanup();

		/// Get shader module
		const VkShaderModule& GetShaderModule() const;
		
		/// Get shader stage info
		const VkPipelineShaderStageCreateInfo& GetShaderStageInfo() const;
	};

}

#endif
