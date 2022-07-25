#include "Engine/Device/Device.hpp"

#include <stdexcept>
#include <set>
#include <iostream>

#include "Engine/Window/Window.hpp"
#include "Engine/SwapChain/SwapChainCreateInfo.hpp"
#include "Engine/SwapChain/SwapChainSupportDetails.hpp"
#include "Engine/CommandPool/CommandPoolCreateInfo.hpp"
#include "Engine/CommandBuffer/CommandBufferCreateInfo.hpp"
#include "Engine/Shader/Shader.hpp"

using namespace RenderEngine;

void Device::InitalizeDevice(const DeviceCreateInfo& _createInfo, Device* _output)
{
	_output->instance = _createInfo.instance;
	_output->surface = _createInfo.surface;
	_output->window = _createInfo.window;

	_output->PickPhysicalDevice();
	_output->CreateLogicalDevice();
	_output->CreateSwapChain();
	_output->CreateRenderPass();
	_output->CreateGraphicsPipeline();
	_output->CreateFrameBuffer();
	_output->CreateCommandPool();
	_output->CreateCommandBuffer();
}


bool Device::IsDeviceSuitable(const VkPhysicalDevice& _device)
{
	QueueFamilyIndices indices = FindQueueFamilies(_device);

	bool extensionsSupported = checkDeviceExtensionSupport(_device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = swapChain.QuerySwapChainSupport(_device, *surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool Device::checkDeviceExtensionSupport(const VkPhysicalDevice& device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

QueueFamilyIndices Device::FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface->GetVkSurface(), &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

void Device::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) 
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (IsDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) 
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

void Device::CreateLogicalDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) 
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create logical device!");
	}

	queueFamilyIndices = indices;
	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
}

void Device::CreateSwapChain()
{
	SwapChainCreateInfo createInfo;
	createInfo.physicalDevice = physicalDevice;
	createInfo.logicalDevice = logicalDevice;
	createInfo.surface = surface;
	createInfo.window = window;
	createInfo.queueFamilyIndices = queueFamilyIndices;

	SwapChain::InitializeSwapChain(createInfo, &swapChain);
}

void Device::CreateRenderPass()
{
	RenderPassCreateInfo createInfo;
	createInfo.logicalDevice = logicalDevice;
	createInfo.swapChainImageFormat = swapChain.GetSwapChainImageFormat();

	RenderPass::InitializeRenderPass(createInfo, &renderPass);
}

void Device::CreateGraphicsPipeline()
{
	ShaderCreateInfo vertexShaderCreateInfo(ShaderType::VERTEX_SHADER, "Resources/Shaders/VertexShader.spv", logicalDevice);
	ShaderCreateInfo fragmentShaderCreateInfo(ShaderType::FRAGMENT_SHADER, "Resources/Shaders/FragmentShader.spv", logicalDevice);

	GraphicsPipelineCreateInfo pipelineInfo;
	Shader::CreateShader(vertexShaderCreateInfo, &pipelineInfo.vertexShader);
	Shader::CreateShader(fragmentShaderCreateInfo, &pipelineInfo.fragmentShader);
	pipelineInfo.swapChainExtent = swapChain.GetSwapChainExtent();
	pipelineInfo.swapChainImageFormat = swapChain.GetSwapChainImageFormat();
	pipelineInfo.logicalDevice = logicalDevice;
	pipelineInfo.renderPass = &renderPass;
	GraphicsPipeline::InitalizeGraphicsPipeline(pipelineInfo, &graphicsPipeline);
}

void Device::CreateFrameBuffer()
{
	FrameBufferCreateInfo createInfo;
	createInfo.logicalDevice = logicalDevice;
	createInfo.renderPass = &renderPass;
	createInfo.imageView = &swapChain.GetImageView();
	createInfo.swapChainImageCount = swapChain.GetImageImageCount();
	createInfo.swapChainExtent = swapChain.GetSwapChainExtent();

	FrameBuffer::InitializeFrameBuffer(createInfo, &frameBuffer);
}

void Device::CreateCommandPool()
{
	CommandPoolCreateInfo createInfo;
	createInfo.logicalDevice = logicalDevice;
	createInfo.graphicsQueueIndex = queueFamilyIndices.graphicsFamily.value();
	CommandPool::InitializeCommandPool(createInfo, &commandPool);
}

void Device::CreateCommandBuffer()
{
	CommandBufferCreateInfo createInfo;
	createInfo.logicalDevice = logicalDevice;
	createInfo.commandPool = &commandPool;
	createInfo.renderPass = &renderPass;
	createInfo.graphicsPipeline = &graphicsPipeline;
	createInfo.frameBuffer = &frameBuffer;
	createInfo.window = window;

	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		SwapChainCommandBuffer::InitializeCommandBuffer(createInfo, &commandBuffers[i]);
	}
}

void Device::CleanUpSwapChain()
{
	frameBuffer.Cleanup();
	swapChain.Cleanup();
}

void Device::RecreateSwapChain()
{
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

void Device::DrawFrame()
{
	vkWaitForFences(logicalDevice, 1, &commandBuffers[currentFrame].GetInFlightFence(), VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain.GetVKSwapChain(), UINT64_MAX, commandBuffers[currentFrame].GetImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		RecreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

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

	VkSemaphore signalSemaphores[] = { commandBuffers[currentFrame].GetRenderFinishedSemaphore()};
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

	VkSwapchainKHR swapChains[] = { swapChain.GetVKSwapChain()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->GetFrameBufferResized()) 
	{
		window->SetFrameBufferResized(false);
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

const VkPhysicalDevice& Device::GetPhysicalDevice() const
{
	return physicalDevice;
}

const uint32_t& Device::GetGraphicsQueueIndex()const 
{
	return queueFamilyIndices.graphicsFamily.value();
}

const VkDevice& Device::GetLogicalDevice() const 
{
	return logicalDevice;
}

const VkQueue& Device::GetGraphicsQueue() const
{
	return graphicsQueue;
}

void Device::Cleanup()
{
	std::cout << "[Cleaning] Device" << std::endl;
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		commandBuffers[i].Cleanup();
	}
	commandPool.Cleanup();
	frameBuffer.Cleanup();
	graphicsPipeline.Cleanup();
	renderPass.Cleanup();
	swapChain.Cleanup();
	vkDestroyDevice(logicalDevice, nullptr);
	std::cout << "[Cleaned] Device" << std::endl;
}