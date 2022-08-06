#include "Vulkan/DeviceContext/DeviceContext.hpp"

#include <stdexcept>
#include <set>
#include <iostream>

#include "Vulkan/SwapChain/SwapChainCreateInfo.hpp"
#include "Vulkan/Shader/Shader.hpp"
#include "Vulkan/SwapChain/SwapChain.hpp"
#include "Vulkan/RenderContext/RenderContextCreateInfo.hpp"

using namespace RenderEngine::Vulkan;

void DeviceContext::InitalizeDevice(const DeviceContextCreateInfo& _createInfo, DeviceContext* _output)
{
	_output->instance = _createInfo.instance;
	_output->window = _createInfo.window;

	_output->CreateSurface();
	_output->PickPhysicalDevice();
	_output->CreateLogicalDevice();

	RenderContextCreateInfo createInfo;
	createInfo.instance = _output->instance;
	createInfo.surface = &_output->surface;
	createInfo.window = _output->window;
	createInfo.physicalDevice = _output->physicalDevice;
	createInfo.logicalDevice = _output->logicalDevice;
	createInfo.queueFamilyIndices = _output->queueFamilyIndices;
	createInfo.graphicsQueue = _output->graphicsQueue;
	createInfo.presentQueue = _output->presentQueue;

	RenderContext::InitalizeRenderContext(createInfo, &_output->renderContext);
}

void DeviceContext::CreateSurface()
{
	Surface::InitializeSurface(instance, window, &surface);
}

bool DeviceContext::IsDeviceSuitable(const VkPhysicalDevice& _device)
{
	QueueFamilyIndices indices = FindQueueFamilies(_device);

	bool extensionsSupported = checkDeviceExtensionSupport(_device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = SwapChain::QuerySwapChainSupport(_device, surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool DeviceContext::checkDeviceExtensionSupport(const VkPhysicalDevice& device)
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

QueueFamilyIndices DeviceContext::FindQueueFamilies(VkPhysicalDevice device)
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
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface.GetVkSurface(), &presentSupport);

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

void DeviceContext::PickPhysicalDevice()
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

void DeviceContext::CreateLogicalDevice()
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


const VkPhysicalDevice& DeviceContext::GetPhysicalDevice() const
{
	return physicalDevice;
}

const uint32_t& DeviceContext::GetGraphicsQueueIndex()const
{
	return queueFamilyIndices.graphicsFamily.value();
}

const VkDevice& DeviceContext::GetLogicalDevice() const
{
	return logicalDevice;
}

const VkQueue& DeviceContext::GetGraphicsQueue() const
{
	return graphicsQueue;
}

RenderContext* DeviceContext::GetRenderContext()
{
	return &renderContext;
}

void DeviceContext::Cleanup()
{
	renderContext.Cleanup();
	vkDestroyDevice(logicalDevice, nullptr);
	surface.Cleanup();
	std::cout << "[Cleaned] Device Context" << std::endl;
}