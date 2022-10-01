#include "Engine/Vulkan/RenderContext/RenderContext.hpp"

#include <iostream>
#include "Core/Vertrex/Vertex.hpp"

using namespace RenderEngine::Engine::Vulkan;
using namespace RenderEngine::Core;

void RenderContext::InitalizeRenderContext(const RenderContextVkCreateInfo& _createInfo, RenderContext* _output)
{
	_output->instance = _createInfo.instance;
	_output->windowProperties = _createInfo.windowProperties;
	_output->physicalDevice = _createInfo.physicalDevice;
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->queueFamilyIndices = _createInfo.queueFamilyIndices;
	_output->graphicsQueue = _createInfo.graphicsQueue;
	_output->presentQueue = _createInfo.presentQueue;

	_output->windowProperties->window->FramebufferResizeEvent.Add(_output, &RenderContext::FrameBufferResizedCallback);

	_output->CreateSwapChain();
	_output->CreateRenderPass();
	_output->CreateDescriptorLayout();
	_output->CreateDescriptorPool();
	_output->CreateGraphicsPipeline(*_createInfo.renderContextCreateInfo.graphicsPipelineCreateInfo);
	_output->CreateFrameBuffer();
	_output->CreateCommandPool();
	_output->CreateCommandBuffer(*_createInfo.renderContextCreateInfo.swapChainCommandBufferCreateInfo);
}

void RenderContext::CreateSwapChain()
{
	SwapChainVkCreateInfo createInfo;
	createInfo.physicalDevice = physicalDevice;
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

	RenderPass::InitializeRenderPass(createInfo, &renderPass);
}

void RenderContext::CreateGraphicsPipeline(const GraphicsPipelineCreateInfo& _createInfo)
{
	GraphicsPipelineVkCreateInfo pipelineInfo{};
	pipelineInfo.swapChainExtent = swapChain.GetExtent();
	pipelineInfo.swapChainImageFormat = swapChain.GetImageFormat();
	pipelineInfo.logicalDevice = logicalDevice;
	pipelineInfo.renderPass = &renderPass;
	pipelineInfo.descriptorLayout = &descriptorSetLayout;
	pipelineInfo.graphicsPipelineCreateInfo = _createInfo;
	GraphicsPipeline::InitalizeGraphicsPipeline(pipelineInfo, &graphicsPipeline);
}

void RenderContext::CreateFrameBuffer()
{
	FrameBufferVkCreateInfo createInfo{};
	createInfo.logicalDevice = logicalDevice;
	createInfo.renderPass = &renderPass;
	createInfo.imageView = &swapChain.GetImageView();
	createInfo.swapChainImageCount = swapChain.GetImageCount();
	createInfo.swapChainExtent = swapChain.GetExtent();

	FrameBuffer::InitializeFrameBuffer(createInfo, &frameBuffer);
}

void RenderContext::CreateCommandPool()
{
	CommandPoolVkCreateInfo createInfo{};
	createInfo.logicalDevice = logicalDevice;
	createInfo.graphicsQueueIndex = queueFamilyIndices.graphicsFamily.value();
	CommandPool::InitializeCommandPool(createInfo, &commandPool);
}

void RenderContext::CreateCommandBuffer(const SwapChainCommandBufferCreateInfo& _createInfo)
{
	SwapChainCommandBufferVkCreateInfo createInfo{};
	createInfo.commandBufferCreateInfo = _createInfo;
	createInfo.logicalDevice = logicalDevice;
	createInfo.commandPool = &commandPool;
	createInfo.renderPass = &renderPass;
	createInfo.graphicsPipeline = &graphicsPipeline;
	createInfo.frameBuffer = &frameBuffer;
	createInfo.swapChain = &swapChain;

	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		SwapChainCommandBuffer::InitializeCommandBuffer(createInfo, &commandBuffers[i]);
	}
}

void RenderContext::CreateDescriptorLayout()
{
	DescriptorSetLayoutVkCreateInfo createInfo{};
	createInfo.logicalDevice = logicalDevice;

	DescriptorSetLayout::InitializeDescriptorSetLayout(createInfo, &descriptorSetLayout);
}

void RenderContext::CreateDescriptorPool()
{
	DescriptorPoolVkCreateInfo createInfo{};
	createInfo.logicalDevice = logicalDevice;

	DescriptorPool::InitializeDescriptorPool(createInfo, &descriptorPool);
}

void RenderContext::CleanUpSwapChain()
{
	frameBuffer.Cleanup();
	swapChain.Cleanup();
}

void RenderContext::RecreateSwapChain()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(windowProperties->window->GetGLFWWindow(), &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(windowProperties->window->GetGLFWWindow(), &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(logicalDevice);

	CleanUpSwapChain();

	CreateSwapChain();
	CreateFrameBuffer();

	frameBufferWasResized = false;
}

void RenderContext::FrameBufferResizedCallback()
{
	frameBufferWasResized = true;
}

void RenderContext::DrawScene(RenderEngine::Core::Scene* _scene)
{
	VkScene* vkScene = WasSceneLoaded(_scene);

	if (vkScene == nullptr)
	{
		vkScene = LoadScene(_scene);
	}

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

VkScene* RenderContext::LoadScene(RenderEngine::Core::Scene* _scene)
{
	SceneData* data = &scenesData.emplace_front();
	data->scene = _scene;

	VkSceneCreateInfo createInfo{};
	createInfo.commandPool = &commandPool;
	createInfo.graphicsQueue = graphicsQueue;
	createInfo.logicalDevice = logicalDevice;
	createInfo.physicalDevice = physicalDevice;
	createInfo.descriptorSetLayout = &descriptorSetLayout;
	createInfo.descriptorPool = &descriptorPool;
	createInfo.graphicsPipeline = &graphicsPipeline;
	createInfo.scene = _scene;

	data->vkScene = VkScene(createInfo);

	return &data->vkScene;
}

VkScene* RenderContext::WasSceneLoaded(RenderEngine::Core::Scene* _scene)
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
	commandPool.Cleanup();
	frameBuffer.Cleanup();
	graphicsPipeline.Cleanup();
	descriptorPool.Cleanup();
	descriptorSetLayout.Cleanup();
	renderPass.Cleanup();
	swapChain.Cleanup();
	std::cout << "[Cleaned] Render Context" << std::endl;
}