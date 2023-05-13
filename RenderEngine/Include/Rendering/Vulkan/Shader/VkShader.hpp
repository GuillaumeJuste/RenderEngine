#pragma once

#ifndef RENDERENGINE_VKSHADER
#define RENDERENGINE_VKSHADER

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Shader/VkShaderCreateInfo.hpp"
#include "Rendering/Base/Interface/Primitive/IShader.hpp"

namespace RenderEngine::Rendering
{
	/**
	 * @brief Class to manage creation and usage of a shader
	*/
	class VkShader : public IShader
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

		static VkShaderStageFlagBits EnumToVkFlag(RenderEngine::Assets::SHADER_STAGE _stage);

	public:
		/// default constructor
		VkShader() = default;

		/// default destructor
		~VkShader() = default;

		/**
		 * @brief Create shader
		 * @param _createInfo shader create info
		 * @param _output shader to initilize
		*/
		static void CreateVkShader(VkShaderCreateInfo _createInfo, VkShader* _output);
		
		/**
		 * @brief Clean up vulkan classes
		*/
		void Clean();

		/// Get shader module
		const VkShaderModule& GetShaderModule() const;
		
		/// Get shader stage info
		const VkPipelineShaderStageCreateInfo& GetShaderStageInfo() const;
	};

}

#endif
