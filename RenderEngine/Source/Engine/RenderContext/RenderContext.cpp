#include "Engine/RenderContext/RenderContext.hpp"

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

void RenderContext::InitalizeRenderContext(const RenderContextCreateInfo& _createInfo, RenderContext* _output)
{
	_output->instance = _createInfo.instance;
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->physicalDevice = _createInfo.physicalDevice;
	_output->queueFamilyIndices = _createInfo.queueFamilyIndices;
	_output->surface = _createInfo.surface;
	_output->window = _createInfo.window;
	_output->graphicsQueue = _createInfo.graphicsQueue;
	_output->presentQueue = _createInfo.presentQueue;

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
	SwapChainCreateInfo createInfo;
	createInfo.physicalDevice = physicalDevice;
	createInfo.logicalDevice = logicalDevice;
	createInfo.surface = surface;
	createInfo.window = window;
	createInfo.queueFamilyIndices = queueFamilyIndices;

	SwapChain::InitializeSwapChain(createInfo, &swapChain);
}

void RenderContext::CreateRenderPass()
{
	RenderPassCreateInfo createInfo;
	createInfo.logicalDevice = logicalDevice;
	createInfo.swapChainImageFormat = swapChain.GetSwapChainImageFormat();

	RenderPass::InitializeRenderPass(createInfo, &renderPass);
}

void RenderContext::CreateGraphicsPipeline()
{
	ShaderCreateInfo vertexShaderCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, "Resources/Shaders/VertexShader.spv", logicalDevice);
	ShaderCreateInfo fragmentShaderCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, "Resources/Shaders/FragmentShader.spv", logicalDevice);

	GraphicsPipelineCreateInfo pipelineInfo;
	Shader::CreateShader(vertexShaderCreateInfo, &pipelineInfo.vertexShader);
	Shader::CreateShader(fragmentShaderCreateInfo, &pipelineInfo.fragmentShader);
	pipelineInfo.swapChainExtent = swapChain.GetSwapChainExtent();
	pipelineInfo.swapChainImageFormat = swapChain.GetSwapChainImageFormat();
	pipelineInfo.logicalDevice = logicalDevice;
	pipelineInfo.renderPass = &renderPass;
	GraphicsPipeline::InitalizeGraphicsPipeline(pipelineInfo, &graphicsPipeline);
}

void RenderContext::CreateFrameBuffer()
{
	FrameBufferCreateInfo createInfo;
	createInfo.logicalDevice = logicalDevice;
	createInfo.renderPass = &renderPass;
	createInfo.imageView = &swapChain.GetImageView();
	createInfo.swapChainImageCount = swapChain.GetImageImageCount();
	createInfo.swapChainExtent = swapChain.GetSwapChainExtent();

	FrameBuffer::InitializeFrameBuffer(createInfo, &frameBuffer);
}

void RenderContext::CreateCommandPool()
{
	CommandPoolCreateInfo createInfo;
	createInfo.logicalDevice = logicalDevice;
	createInfo.graphicsQueueIndex = queueFamilyIndices.graphicsFamily.value();
	CommandPool::InitializeCommandPool(createInfo, &commandPool);
}

void RenderContext::CreateCommandBuffer()
{
	CommandBufferCreateInfo createInfo;
	createInfo.logicalDevice = logicalDevice;
	createInfo.commandPool = &commandPool;
	createInfo.renderPass = &renderPass;
	createInfo.graphicsPipeline = &graphicsPipeline;
	createInfo.frameBuffer = &frameBuffer;
	createInfo.window = window;
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
	vkDeviceWaitIdle(logicalDevice);

	int width = 0, height = 0;
	glfwGetFramebufferSize(window->GetGLFWWindow(), &width, &height);
	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window->GetGLFWWindow(), &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(logicalDevice);

	CleanUpSwapChain();

	CreateSwapChain();
	CreateFrameBuffer();
}

void RenderContext::CreateVertexBufferObject()
{
	BufferObjectCreateInfo stagingBufferCreateInfo;
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

	BufferObjectCreateInfo vertexBuffeCreateInfo;
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
	BufferObjectCreateInfo stagingBufferCreateInfo;
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

	BufferObjectCreateInfo indexBufferCreateInfo;
	indexBufferCreateInfo.physicalDevice = physicalDevice;
	indexBufferCreateInfo.logicalDevice = logicalDevice;
	indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	indexBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	indexBufferCreateInfo.bufferSize = stagingBufferCreateInfo.bufferSize;

	BufferObject::InitializeBufferObject(indexBufferCreateInfo, &indexBufferObject);

	stagingBufferObject.CopyBuffer(&indexBufferObject, &commandPool, graphicsQueue, stagingBufferCreateInfo.bufferSize);
	stagingBufferObject.Cleanup();
}

void RenderContext::DrawFrame()
{
	vkWaitForFences(logicalDevice, 1, &commandBuffers[currentFrame].GetInFlightFence(), VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain.GetVKSwapChain(), UINT64_MAX, commandBuffers[currentFrame].GetImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);

	/*TODO: Fix window resize fence error*/
	/*if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		RecreateSwapChain();
		vkAcquireNextImageKHR(logicalDevice, swapChain.GetVKSwapChain(), UINT64_MAX, commandBuffers[currentFrame].GetImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}*/

	vkResetFences(logicalDevice, 1, &commandBuffers[currentFrame].GetInFlightFence());


	vkResetCommandBuffer(commandBuffers[currentFrame].GetVKCommandBuffer(), /*VkCommandBufferResetFlagBits*/ 0);
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
	/*if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->GetFrameBufferResized())
	{
		window->SetFrameBufferResized(false);
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}*/

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void RenderContext::Cleanup()
{
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