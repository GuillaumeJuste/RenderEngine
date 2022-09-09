#include "Vulkan/RenderContext/RenderContext.hpp"

#include <iostream>
#include "Core/Vertrex/Vertex.hpp"

using namespace RenderEngine::Vulkan;
using namespace RenderEngine::Core;

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
};

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
	_output->CreateGraphicsPipeline();
	_output->CreateFrameBuffer();
	_output->CreateCommandPool();
	_output->CreateVertexBufferObject();
	_output->CreateIndexBufferObject();
	_output->CreateCommandBuffer();
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

void RenderContext::CreateGraphicsPipeline()
{
	ShaderVkCreateInfo vertexShaderCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, "Resources/Shaders/VertexShader.spv", logicalDevice);
	ShaderVkCreateInfo fragmentShaderCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, "Resources/Shaders/FragmentShader.spv", logicalDevice);

	GraphicsPipelineVkCreateInfo pipelineInfo{};
	Shader::CreateShader(vertexShaderCreateInfo, &pipelineInfo.vertexShader);
	Shader::CreateShader(fragmentShaderCreateInfo, &pipelineInfo.fragmentShader);
	pipelineInfo.swapChainExtent = swapChain.GetExtent();
	pipelineInfo.swapChainImageFormat = swapChain.GetImageFormat();
	pipelineInfo.logicalDevice = logicalDevice;
	pipelineInfo.renderPass = &renderPass;
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

void RenderContext::CreateCommandBuffer()
{
	SwapChainCommandBufferVkCreateInfo createInfo{};
	createInfo.logicalDevice = logicalDevice;
	createInfo.commandPool = &commandPool;
	createInfo.renderPass = &renderPass;
	createInfo.graphicsPipeline = &graphicsPipeline;
	createInfo.frameBuffer = &frameBuffer;
	createInfo.swapChain = &swapChain;

	createInfo.vertexBufferObject = &vertexBufferObject;
	createInfo.indexBufferObject = &indexBufferObject;

	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		SwapChainCommandBuffer::InitializeCommandBuffer(createInfo, &commandBuffers[i]);
	}
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

void RenderContext::CreateVertexBufferObject()
{
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = physicalDevice;
	stagingBufferCreateInfo.logicalDevice = logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = sizeof(vertices[0]) * vertices.size();

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBufferObject);

	void* data;
	vkMapMemory(logicalDevice, stagingBufferObject.GetVkBufferMemory(), 0, stagingBufferCreateInfo.bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)stagingBufferCreateInfo.bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferObject.GetVkBufferMemory());

	BufferObjectVkCreateInfo vertexBuffeCreateInfo;
	vertexBuffeCreateInfo.physicalDevice = physicalDevice;
	vertexBuffeCreateInfo.logicalDevice = logicalDevice;
	vertexBuffeCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	vertexBuffeCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	vertexBuffeCreateInfo.bufferSize = stagingBufferCreateInfo.bufferSize;

	BufferObject::InitializeBufferObject(vertexBuffeCreateInfo, &vertexBufferObject);

	stagingBufferObject.CopyBuffer(&vertexBufferObject, &commandPool, graphicsQueue, stagingBufferCreateInfo.bufferSize);
	stagingBufferObject.Cleanup();
}

void RenderContext::CreateIndexBufferObject()
{
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = physicalDevice;
	stagingBufferCreateInfo.logicalDevice = logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = sizeof(indices[0]) * indices.size();

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBufferObject);

	void* data;
	vkMapMemory(logicalDevice, stagingBufferObject.GetVkBufferMemory(), 0, stagingBufferCreateInfo.bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)stagingBufferCreateInfo.bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferObject.GetVkBufferMemory());

	BufferObjectVkCreateInfo indexBufferCreateInfo;
	indexBufferCreateInfo.physicalDevice = physicalDevice;
	indexBufferCreateInfo.logicalDevice = logicalDevice;
	indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	indexBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	indexBufferCreateInfo.bufferSize = stagingBufferCreateInfo.bufferSize;

	BufferObject::InitializeBufferObject(indexBufferCreateInfo, &indexBufferObject);

	stagingBufferObject.CopyBuffer(&indexBufferObject, &commandPool, graphicsQueue, stagingBufferCreateInfo.bufferSize);
	stagingBufferObject.Cleanup();
}

void RenderContext::FrameBufferResizedCallback()
{
	frameBufferWasResized = true;
}

void RenderContext::DrawFrame()
{
	vkWaitForFences(logicalDevice, 1, &commandBuffers[currentFrame].GetInFlightFence(), VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain.GetVKSwapChain(), UINT64_MAX, commandBuffers[currentFrame].GetImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);

	/*TODO: Fix window resize fence error*/
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
	commandBuffers[currentFrame].RecordCommandBuffer(imageIndex);

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

	/*TODO: Fix window resize fence error*/
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || frameBufferWasResized)
	{
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void RenderContext::Cleanup()
{
	windowProperties->window->FramebufferResizeEvent.Remove(this, &RenderContext::FrameBufferResizedCallback);
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		commandBuffers[i].Cleanup();
	}
	commandPool.Cleanup();
	frameBuffer.Cleanup();
	graphicsPipeline.Cleanup();
	renderPass.Cleanup();
	swapChain.Cleanup();
	indexBufferObject.Cleanup();
	vertexBufferObject.Cleanup();
	std::cout << "[Cleaned] Render Context" << std::endl;
}