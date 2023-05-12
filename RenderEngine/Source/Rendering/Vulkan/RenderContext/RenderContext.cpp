#include "Rendering/Vulkan/RenderContext/RenderContext.hpp"

#include <iostream>
#include "ResourceManager/Assets/Mesh/Vertex.hpp"

using namespace RenderEngine::Rendering;
using namespace RenderEngine::Assets;

void RenderContext::InitalizeRenderContext(const RenderContextVkCreateInfo& _createInfo, RenderContext* _output)
{
	_output->instance = _createInfo.instance;
	_output->windowProperties = _createInfo.windowProperties;
	_output->physicalDevice = _createInfo.physicalDevice;
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->queueFamilyIndices = _createInfo.queueFamilyIndices;
	_output->graphicsQueue = _createInfo.graphicsQueue;
	_output->presentQueue = _createInfo.presentQueue;
	_output->commandPool = _createInfo.commandPool;

	_output->windowProperties->window->FramebufferResizeEvent.Add(_output, &RenderContext::FrameBufferResizedCallback);

	_output->CreateSwapChain();
	_output->CreateDepthBuffer();
	_output->CreateRenderPass();
	_output->CreateFrameBuffer();
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
	createInfo.depthBuffer = &depthBuffer;

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

	FrameBuffer::InitializeFrameBuffer(createInfo, &frameBuffer);
}

void RenderContext::CreateDepthBuffer()
{
	DepthBufferVkCreateInfo createInfo{};
	createInfo.physicalDevice = physicalDevice;
	createInfo.logicalDevice = logicalDevice;
	createInfo.commandPool = commandPool;
	createInfo.graphicsQueue = graphicsQueue;
	createInfo.swapChainExtent = swapChain.GetExtent();
	
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
	createInfo.physicalDevice = physicalDevice;
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

bool RenderContext::CreateMesh(const RenderEngine::Assets::RawMesh& _input, RenderEngine::Assets::Mesh& _output)
{
	CreateVertexBufferObject(_input, _output);
	CreateIndexBufferObject(_input, _output);
	return true;
}

void RenderContext::CreateVertexBufferObject(const RenderEngine::Assets::RawMesh& _input, RenderEngine::Assets::Mesh& _output)
{
	BufferObject stagingBufferObject;
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = physicalDevice;
	stagingBufferCreateInfo.logicalDevice = logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = sizeof(_input.vertices[0]) * _input.vertices.size();

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBufferObject);

	void* data;
	vkMapMemory(logicalDevice, stagingBufferObject.GetVkBufferMemory(), 0, stagingBufferCreateInfo.bufferSize, 0, &data);
	memcpy(data, _input.vertices.data(), (size_t)stagingBufferCreateInfo.bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferObject.GetVkBufferMemory());

	BufferObjectVkCreateInfo vertexBuffeCreateInfo;
	vertexBuffeCreateInfo.physicalDevice = physicalDevice;
	vertexBuffeCreateInfo.logicalDevice = logicalDevice;
	vertexBuffeCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	vertexBuffeCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	vertexBuffeCreateInfo.bufferSize = stagingBufferCreateInfo.bufferSize;

	BufferObject* VBO = new BufferObject();

	BufferObject::InitializeBufferObject(vertexBuffeCreateInfo, VBO);

	stagingBufferObject.CopyBuffer(VBO, commandPool, graphicsQueue, stagingBufferCreateInfo.bufferSize);
	stagingBufferObject.Clean();

	_output.vertexBuffer = VBO;
}

void RenderContext::CreateIndexBufferObject(const RenderEngine::Assets::RawMesh& _input, RenderEngine::Assets::Mesh& _output)
{
	BufferObject stagingBufferObject;
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = physicalDevice;
	stagingBufferCreateInfo.logicalDevice = logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = sizeof(_input.indices[0]) * _input.indices.size();

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBufferObject);

	void* data;
	vkMapMemory(logicalDevice, stagingBufferObject.GetVkBufferMemory(), 0, stagingBufferCreateInfo.bufferSize, 0, &data);
	memcpy(data, _input.indices.data(), (size_t)stagingBufferCreateInfo.bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferObject.GetVkBufferMemory());

	BufferObjectVkCreateInfo indexBufferCreateInfo;
	indexBufferCreateInfo.physicalDevice = physicalDevice;
	indexBufferCreateInfo.logicalDevice = logicalDevice;
	indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	indexBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	indexBufferCreateInfo.bufferSize = stagingBufferCreateInfo.bufferSize;

	BufferObject* IBO = new BufferObject();

	BufferObject::InitializeBufferObject(indexBufferCreateInfo, IBO);

	stagingBufferObject.CopyBuffer(IBO, commandPool, graphicsQueue, stagingBufferCreateInfo.bufferSize);
	stagingBufferObject.Clean();

	_output.indexBuffer = IBO;
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
	renderPass.Cleanup();
	swapChain.Cleanup();
}