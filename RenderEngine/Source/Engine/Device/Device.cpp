#include "Engine/Device/Device.hpp"

#include <stdexcept>
#include <set>

#include "Engine/Window/Window.hpp"
#include "Engine/SwapChain/SwapChainCreateInfo.hpp"
#include "Engine/SwapChain/SwapChainSupportDetails.hpp"
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
	_output->CreateGraphicsPipeline();
}


bool Device::IsDeviceSuitable(const VkPhysicalDevice& _device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(_device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(_device, &deviceFeatures);

	bool extensionsSupported = checkDeviceExtensionSupport(_device);

	bool swapChainAdequate = false;
	if (extensionsSupported) 
	{
		SwapChainSupportDetails swapChainSupport = SwapChain::QuerySwapChainSupport(_device, surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
		&& deviceFeatures.geometryShader 
		&& extensionsSupported
		&& swapChainAdequate;
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

	for (const auto& device : devices)
	{
		if (IsDeviceSuitable(device))
		{
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	graphicsQueueIndex = UINT32_MAX;

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			VkBool32 canPresentSurface;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface.GetVkSurface(), &canPresentSurface);
			if (canPresentSurface)
			{
				graphicsQueueIndex = i;
				break;
			}
		}

		i++;
	}
}

void Device::CreateLogicalDevice()
{
	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(logicalDevice, graphicsQueueIndex, 0, &graphicsQueue);
	presentQueue = graphicsQueue;
}

void Device::CreateSwapChain()
{
	SwapChainCreateInfo createInfo;
	createInfo.physicalDevice = physicalDevice;
	createInfo.logicalDevice = logicalDevice;
	createInfo.surface = surface;
	createInfo.window = window;

	SwapChain::InitializeSwapChain(createInfo, &swapChain);
}

void Device::CreateGraphicsPipeline()
{
	ShaderCreateInfo vertexShaderCreateInfo(ShaderType::VERTEX_SHADER, "Resources/Shaders/VertexShader.spv", logicalDevice);
	ShaderCreateInfo fragmentShaderCreateInfo(ShaderType::FRAGMENT_SHADER, "Resources/Shaders/FragmentShader.spv", logicalDevice);

	GraphicsPipelineCreateInfo pipelineInfo;
	Shader::CreateShader(vertexShaderCreateInfo, &pipelineInfo.vertexShader);
	Shader::CreateShader(fragmentShaderCreateInfo, &pipelineInfo.fragmentShader);
	pipelineInfo.swapChainExtent = swapChain.GetSwapChainExtent();
	pipelineInfo.logicalDevice = &logicalDevice;
	GraphicsPipeline::InitalizeGraphicsPipeline(pipelineInfo, &graphicsPipeline);
}

const VkPhysicalDevice& Device::GetPhysicalDevice() const
{
	return physicalDevice;
}

const uint32_t& Device::GetGraphicsQueueIndex()const 
{
	return graphicsQueueIndex;
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
	graphicsPipeline.Cleanup();
	swapChain.Cleanup();
	vkDestroyDevice(logicalDevice, nullptr);
}