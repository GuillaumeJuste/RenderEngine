#include "Vulkan/VulkanContext/VulkanContext.hpp"

#include <stdexcept>
#include <iostream>

#include "Vulkan/DeviceContext/DeviceContextCreateInfo.hpp"

using namespace RenderEngine::Vulkan;

VulkanContext::VulkanContext() 
{
    CreateInstance();
    SetupDebugMessenger();
}

void VulkanContext::CreateInstance()
{
    if (enableValidationLayers && !CheckValidationLayerSupport()) 
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "RenderEngine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "RenderEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) 
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        DebugMessenger::PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else 
    {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    auto extensions = GetRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create instance!");
    }
}

void VulkanContext::SetupDebugMessenger()
{
    if (!enableValidationLayers)
        return;

    DebugMessenger::InitializeDebugMessenger(instance, &debugMessenger);
}

bool VulkanContext::CheckValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

std::vector<const char*> VulkanContext::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

WindowProperties* VulkanContext::AddWindow(Window* _window)
{
    for (std::vector<WindowProperties>::iterator it = windowProperties.begin(); it != windowProperties.end(); ++it)
    {
        if (it->window == _window)
            return &(*it);
    }

    WindowProperties properties;
    properties.window = _window;
    Surface::InitializeSurface(instance, _window, &properties.surface);

    windowProperties.push_back(properties);

    return &windowProperties.back();
}

DeviceContext* VulkanContext::CreateDeviceContext(WindowProperties* _windowProperties)
{
    DeviceContextCreateInfo createInfo;
    createInfo.instance = instance;
    createInfo.windowProperties = _windowProperties;

    DeviceContext newDevice;

    DeviceContext::InitalizeDevice(createInfo, &newDevice);

    deviceContexts.push_back(newDevice);

    return &deviceContexts.back();
}

//RenderContext* VulkanContext::CreateRenderContext()
//{
//    RenderContextCreateInfo createInfo;
//    createInfo.instance = instance;
//    createInfo.window = window;
//
//    DeviceContext deviceContext;
//    deviceContexts.push_back(deviceContext);
//    deviceContext = deviceContexts.back();
//    RenderContext* rendercontext = deviceContext.AddRenderContext(createInfo);
//
//    std::vector<PhysicalDeviceProperties> deviceProperties = DeviceContext::QuerySuitableDevice(instance, rendercontext->GetSurface());
//
//}

void VulkanContext::Cleanup()
{
    int deviceSize = deviceContexts.size();

    for (int i = 0; i < deviceSize; i++)
    {
        deviceContexts[i].Cleanup();
    }

    for (std::vector<WindowProperties>::iterator it = windowProperties.begin(); it != windowProperties.end(); ++it)
    {
        it->surface.Cleanup();
    }

    if (enableValidationLayers) 
    {
        debugMessenger.Cleanup();
    }

    vkDestroyInstance(instance, nullptr);
    std::cout << "[Cleaned] Vulkan Context" << std::endl;
}