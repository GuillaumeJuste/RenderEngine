#pragma once

#ifndef RENDERENGINE_SYNCOBJECTS
#define RENDERENGINE_SYNCOBJECTS

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/SyncObjects/SyncObjectsCreateInfo.hpp"

namespace RenderEngine
{

	class SyncObjects
	{
	private:
		VkDevice logicalDevice;

		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		VkFence inFlightFence;

	public:
		SyncObjects() = default;
		~SyncObjects() = default;

		static void InitializeSyncObjects(SyncObjectsCreateInfo _createInfo, SyncObjects* _output);

		void Cleanup();

		const VkSemaphore& GetImageAvailableSemaphore() const;
		const VkSemaphore& GetRenderFinishedSemaphore() const;
		const VkFence& GetInFlightFence() const;
	};

}

#endif