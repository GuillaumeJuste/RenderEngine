#include "Rendering/Vulkan/Context/DeviceContext/DeviceContext.hpp"

#include <stdexcept>
#include <set>
#include <iostream>

#include "Rendering/Vulkan/SwapChain/SwapChainVkCreateInfo.hpp"
#include "Rendering/Vulkan/SwapChain/SwapChain.hpp"
#include "Rendering/Vulkan/Context/RenderContext/RenderContextVkCreateInfo.hpp"


using namespace RenderEngine::Rendering;

void DeviceContext::CreateDeviceContext(const DeviceContextVkCreateInfo& _createInfo, DeviceContext* _output)
{
	_output->instance = _createInfo.instance;
	_output->windowProperties = _createInfo.windowProperties;
}

std::vector<std::string> DeviceContext::QueryAvailblePhysicalDevices()
{
	std::vector<std::string> physicalDeviceList;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> deviceContexts(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, deviceContexts.data());

	for (const auto& device : deviceContexts)
	{
		PhysicalDeviceProperties properties(device);
		if (IsDeviceSuitable(&properties))
		{
			physicalDevicesProperties.push_back(properties);
			physicalDeviceList.push_back(properties.properties.deviceName);
		}
	}

	if (physicalDeviceList.size() == 0)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	return physicalDeviceList;
}

void DeviceContext::InitializeDeviceContext(std::string _physicalDeviceName)
{
	if(physicalDevice != VK_NULL_HANDLE)
	{
		throw std::runtime_error("Device already initialized");
	}

	PickPhysicalDevice(_physicalDeviceName);
	CreateLogicalDevice();
	CreateCommandPool();
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

void DeviceContext::PickPhysicalDevice(std::string _physicalDeviceName)
{
	if (physicalDevicesProperties.size() == 0)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
	else 
	{
		for (std::vector<PhysicalDeviceProperties>::iterator it = physicalDevicesProperties.begin(); it != physicalDevicesProperties.end(); ++it)
		{
			if (it->properties.deviceName == _physicalDeviceName)
			{
				physicalDeviceProperties = *it;
				physicalDevice = physicalDeviceProperties.physicalDevice;
				break;
			}
		}
	}

	physicalDevicesProperties.clear();

	if (physicalDevice == VK_NULL_HANDLE) 
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

bool DeviceContext::IsDeviceSuitable(PhysicalDeviceProperties* _properties)
{
	VkPhysicalDeviceProperties properties;

	vkGetPhysicalDeviceProperties(_properties->physicalDevice, &properties);
	bool validProperties = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;

	QueueFamilyIndices queueFamilyIndices;
	bool validQueueFamilyIndices = FindQueueFamilies(_properties->physicalDevice, &queueFamilyIndices);

	bool extensionsSupported = checkDeviceExtensionSupport(_properties->physicalDevice);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = SwapChain::QuerySwapChainSupport(_properties->physicalDevice, windowProperties->surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(_properties->physicalDevice, &supportedFeatures);

	if (validProperties && validQueueFamilyIndices && swapChainAdequate && extensionsSupported && supportedFeatures.samplerAnisotropy)
	{
		_properties->properties = properties;
		_properties->queueFamilyIndices = queueFamilyIndices;
		return true;
	}

	return false;
}

bool DeviceContext::FindQueueFamilies(VkPhysicalDevice _device, QueueFamilyIndices* _output)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, queueFamilies.data());

	int i = 0;

	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(_device, i, windowProperties->surface.GetVkSurface(), &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			*_output = indices;
			return true;
		}

		i++;
	}

	return false;
}

void DeviceContext::CreateLogicalDevice()
{
	QueueFamilyIndices indices;
	FindQueueFamilies(physicalDevice, &indices);

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
	deviceFeatures.samplerAnisotropy = VK_TRUE;

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

	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
}

void DeviceContext::CreateCommandPool()
{
	CommandPoolVkCreateInfo createInfo{};
	createInfo.logicalDevice = logicalDevice;
	createInfo.graphicsQueueIndex = physicalDeviceProperties.queueFamilyIndices.graphicsFamily.value();
	CommandPool::InitializeCommandPool(createInfo, &commandPool);
}

const VkPhysicalDevice& DeviceContext::GetPhysicalDevice() const
{
	return physicalDevice;
}

const VkDevice& DeviceContext::GetLogicalDevice() const
{
	return logicalDevice;
}

const VkQueue& DeviceContext::GetGraphicsQueue() const
{
	return graphicsQueue;
}

IRenderContext* DeviceContext::CreateRenderContext(const IRenderContextCreateInfo& _createInfo)
{
	RenderContextVkCreateInfo createInfo;
	createInfo.renderContextCreateInfo = _createInfo;
	createInfo.instance = instance;
	createInfo.windowProperties = windowProperties;
	createInfo.physicalDevice = physicalDevice;
	createInfo.logicalDevice = logicalDevice;
	createInfo.queueFamilyIndices = physicalDeviceProperties.queueFamilyIndices;
	createInfo.graphicsQueue = graphicsQueue;
	createInfo.presentQueue = presentQueue;
	createInfo.commandPool = &commandPool;

	RenderContext* renderContext = &renderContexts.emplace_front();

	RenderContext::InitalizeRenderContext(createInfo, renderContext);

	return renderContext;
}

void DeviceContext::WaitDeviceIdle()
{
	vkDeviceWaitIdle(logicalDevice);
}

void DeviceContext::Cleanup()
{
	for (std::forward_list<RenderContext>::iterator it = renderContexts.begin(); it != renderContexts.end(); ++it)
	{
		it->Cleanup();
	}

	commandPool.Cleanup();

	vkDestroyDevice(logicalDevice, nullptr);
}