#include "Rendering/Vulkan/Context/RenderContext/RenderContext.hpp"

#include <iostream>
#include <array>
#include "Rendering/Vulkan/Misc/Utils.hpp"
#include "Rendering/Vulkan/ResourceManager/VkResourceManager.hpp"

using namespace RenderEngine::Rendering;
using namespace RenderEngine::Assets;

void RenderContext::InitalizeRenderContext(const RenderContextVkCreateInfo& _createInfo, RenderContext* _output)
{
	_output->instance = _createInfo.instance;
	_output->windowProperties = _createInfo.windowProperties;
	_output->physicalDeviceProperties = _createInfo.physicalDeviceProperties;
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->queueFamilyIndices = _createInfo.physicalDeviceProperties.queueFamilyIndices;
	_output->graphicsQueue = _createInfo.graphicsQueue;
	_output->presentQueue = _createInfo.presentQueue;
	_output->commandPool = _createInfo.commandPool;

	_output->windowProperties->window->FramebufferResizeEvent.Add(_output, &RenderContext::FrameBufferResizedCallback);

	_output->CreateSwapChain();
	_output->createColorResources();
	_output->CreateDepthBuffer();
	_output->CreateRenderPass();
	_output->CreateFrameBuffer();
	_output->CreateCommandBuffer(*_createInfo.renderContextCreateInfo.swapChainCommandBufferCreateInfo);

}

ResourceManager* RenderContext::CreateResourceManager()
{
	VkResourceManagerCreateInfo createInfo{
		physicalDeviceProperties.physicalDevice,
		logicalDevice,
		graphicsQueue,
		commandPool
	};

	return new VkResourceManager(createInfo);
}

void RenderContext::CreateSwapChain()
{
	SwapChainVkCreateInfo createInfo;
	createInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	createInfo.logicalDevice = logicalDevice;
	createInfo.windowProperties = windowProperties;
	createInfo.queueFamilyIndices = queueFamilyIndices;

	SwapChain::InitializeSwapChain(createInfo, &swapChain);
}

void RenderContext::CreateRenderPass()
{
	RenderPassVkCreateInfo createInfo{};
	createInfo.logicalDevice = logicalDevice;
	createInfo.swapChainImageFormat = swapChain.GetImageFormat();
	createInfo.depthBuffer = &depthBuffer;
	createInfo.samples = physicalDeviceProperties.msaaSamples;
	createInfo.createColorResolveAttachement = true;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcAccessMask = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	createInfo.subpassDependencies.push_back(dependency);

	RenderPass::InitializeRenderPass(createInfo, &renderPass);
}

void RenderContext::CreateFrameBuffer()
{
	FrameBufferVkCreateInfo createInfo{};
	createInfo.logicalDevice = logicalDevice;
	createInfo.renderPass = &renderPass;
	createInfo.imageViews = swapChain.GetImageViews();
	createInfo.swapChainImageCount = swapChain.GetImageCount();
	createInfo.swapChainExtent = swapChain.GetExtent();
	createInfo.depthBuffer = &depthBuffer;
	createInfo.colorImage = &colorImage;

	FrameBuffer::InitializeFrameBuffer(createInfo, &frameBuffer);
}

void RenderContext::createColorResources()
{
	VkFormat colorFormat = swapChain.GetImageFormat();

	VkImageBufferCreateInfo imageCreateInfo{};
	imageCreateInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	imageCreateInfo.logicalDevice = logicalDevice;
	imageCreateInfo.width = static_cast<uint32_t>(swapChain.GetExtent().width);
	imageCreateInfo.height = static_cast<uint32_t>(swapChain.GetExtent().height);
	imageCreateInfo.format = colorFormat;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	imageCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	imageCreateInfo.commandPool = commandPool;
	imageCreateInfo.graphicsQueue = graphicsQueue;
	imageCreateInfo.textureCount = 1;
	imageCreateInfo.imageFlags = 0;
	imageCreateInfo.imageViewType = VK_IMAGE_VIEW_TYPE_2D;
	imageCreateInfo.imageViewAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.samples = physicalDeviceProperties.msaaSamples;
	VkImageBuffer::InitializeImageBuffer(imageCreateInfo, &colorImage);
}

void RenderContext::CreateDepthBuffer()
{
	DepthBufferVkCreateInfo createInfo{};
	createInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	createInfo.logicalDevice = logicalDevice;
	createInfo.commandPool = commandPool;
	createInfo.graphicsQueue = graphicsQueue;
	createInfo.swapChainExtent = swapChain.GetExtent();
	createInfo.samples = physicalDeviceProperties.msaaSamples;
	
	DepthBuffer::InitializeDepthBuffer(createInfo, &depthBuffer);
}

void RenderContext::CreateCommandBuffer(const SwapChainCommandBufferCreateInfo& _createInfo)
{
	SwapChainCommandBufferVkCreateInfo createInfo{};
	createInfo.commandBufferCreateInfo = _createInfo;
	createInfo.logicalDevice = logicalDevice;
	createInfo.commandPool = commandPool;
	createInfo.renderPass = &renderPass;
	createInfo.frameBuffer = &frameBuffer;
	createInfo.swapChain = &swapChain;

	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		SwapChainCommandBuffer::InitializeCommandBuffer(createInfo, &commandBuffers[i]);
	}
}

void RenderContext::CleanUpSwapChain()
{
	depthBuffer.Cleanup();
	colorImage.Cleanup();
	frameBuffer.Cleanup();
	swapChain.Cleanup();
}

void RenderContext::RecreateSwapChain()
{
	int width = 0, height = 0;
	windowProperties->window->GetFrameBufferSize(&width, &height);
	while (width == 0 || height == 0) {
		windowProperties->window->GetFrameBufferSize(&width, &height);
	}

	vkDeviceWaitIdle(logicalDevice);

	CleanUpSwapChain();

	CreateSwapChain();

	createColorResources();

	CreateDepthBuffer();

	CreateFrameBuffer();

	frameBufferWasResized = false;
}

void RenderContext::FrameBufferResizedCallback()
{
	frameBufferWasResized = true;
}

void RenderContext::DrawScene(RenderEngine::SceneGraph::Scene* _scene)
{
	VkScene* vkScene = WasSceneLoaded(_scene);

	if (vkScene == nullptr)
	{
		vkScene = LoadScene(_scene);
	}

	vkScene->Update(currentFrame);

	vkWaitForFences(logicalDevice, 1, &commandBuffers[currentFrame].GetInFlightFence(), VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain.GetVKSwapChain(), UINT64_MAX, commandBuffers[currentFrame].GetImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetFences(logicalDevice, 1, &commandBuffers[currentFrame].GetInFlightFence());

	vkResetCommandBuffer(commandBuffers[currentFrame].GetVKCommandBuffer(), 0);

	commandBuffers[currentFrame].RecordCommandBuffer(imageIndex, currentFrame, vkScene);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { commandBuffers[currentFrame].GetImageAvailableSemaphore() };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame].GetVKCommandBuffer();

	VkSemaphore signalSemaphores[] = { commandBuffers[currentFrame].GetRenderFinishedSemaphore() };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, commandBuffers[currentFrame].GetInFlightFence()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain.GetVKSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || frameBufferWasResized)
	{
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

VkScene* RenderContext::LoadScene(RenderEngine::SceneGraph::Scene* _scene)
{
	SceneData* data = &scenesData.emplace_front();
	data->scene = _scene;

	VkSceneCreateInfo createInfo{};
	createInfo.commandPool = commandPool;
	createInfo.graphicsQueue = graphicsQueue;
	createInfo.logicalDevice = logicalDevice;
	createInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	createInfo.samples = physicalDeviceProperties.msaaSamples;
	createInfo.renderpass = &renderPass;
	createInfo.swapchain = &swapChain;
	createInfo.scene = _scene;

	data->vkScene = VkScene(createInfo);

	return &data->vkScene;
}

VkScene* RenderContext::WasSceneLoaded(RenderEngine::SceneGraph::Scene* _scene)
{
	for (std::forward_list<SceneData>::iterator it = scenesData.begin(); it != scenesData.end(); ++it)
	{
		if ((*it).scene == _scene)
		{
			return &(*it).vkScene;
		}
	}
	return nullptr;
}

void RenderContext::Cleanup()
{
	for (std::forward_list<SceneData>::iterator it = scenesData.begin(); it != scenesData.end(); ++it)
	{
		it->vkScene.Cleanup();
	}

	windowProperties->window->FramebufferResizeEvent.Remove(this, &RenderContext::FrameBufferResizedCallback);
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		commandBuffers[i].Cleanup();
	}
	frameBuffer.Cleanup();
	depthBuffer.Cleanup();
	colorImage.Cleanup();
	renderPass.Cleanup();
	swapChain.Cleanup();
}