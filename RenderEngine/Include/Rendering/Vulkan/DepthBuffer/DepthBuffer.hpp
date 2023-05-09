#pragma once

#ifndef RENDERENGINE_DEPTHBUFFER
#define RENDERENGINE_DEPTHBUFFER

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/DepthBuffer/DepthBufferVkCreateInfo.hpp"
#include "Rendering/Vulkan/ImageView/ImageView.hpp"
#include "Rendering/Vulkan/Image/Image.hpp"

#include <vector>

namespace RenderEngine::Rendering
{
	class DepthBuffer
	{
	private:
		/// physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		/// logical device
		VkDevice logicalDevice;

		Image depthImage;
		ImageView depthImageView;

		VkFormat FindSupportedFormat(const std::vector<VkFormat>& _candidates, VkImageTiling _tiling, VkFormatFeatureFlags _features);

	public:

		/// default constructor
		DepthBuffer() = default;

		/// default destructor
		~DepthBuffer() = default;

		static void InitializeDepthBuffer(const DepthBufferVkCreateInfo& _createInfo, DepthBuffer* _output);
		VkFormat FindDepthFormat();

		const VkImage& GetVkImage() const;
		const VkImageView& GetVkImageView() const;

		void Cleanup();
	};

}

#endif