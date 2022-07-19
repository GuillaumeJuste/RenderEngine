#include "Engine/SyncObjects/SyncObjects.hpp"

#include <iostream>

using namespace RenderEngine;

void SyncObjects::InitializeSyncObjects(SyncObjectsCreateInfo _createInfo, SyncObjects* _output)
{
	_output->logicalDevice = _createInfo.logicalDevice;
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if (vkCreateSemaphore(_createInfo.logicalDevice, &semaphoreInfo, nullptr, &_output->imageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(_createInfo.logicalDevice, &semaphoreInfo, nullptr, &_output->renderFinishedSemaphore) != VK_SUCCESS ||
		vkCreateFence(_createInfo.logicalDevice, &fenceInfo, nullptr, &_output->inFlightFence) != VK_SUCCESS) {
		throw std::runtime_error("failed to create synchronization objects for a frame!");
	}
}

const VkSemaphore& SyncObjects::GetImageAvailableSemaphore() const
{
	return imageAvailableSemaphore;
}

const VkSemaphore& SyncObjects::GetRenderFinishedSemaphore() const
{
	return renderFinishedSemaphore;
}

const VkFence& SyncObjects::GetInFlightFence() const
{
	return inFlightFence;
}

void SyncObjects::Cleanup()
{
	vkDestroySemaphore(logicalDevice, imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(logicalDevice, renderFinishedSemaphore, nullptr);
	vkDestroyFence(logicalDevice, inFlightFence, nullptr);
}