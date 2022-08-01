#include "Engine/Device/DeviceContext.hpp"

#include <stdexcept>
#include <iostream>

#include "Engine/Debugger/DebugMessenger.hpp"
#include "Engine/Device/DeviceCreateInfo.hpp"

using namespace RenderEngine::Vulkan;

DeviceContext::DeviceContext(Window* _window) :
    window{ _window }
{
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    CreateDevice();
}

void DeviceContext::CreateInstance()
{
    if (enableValidationLayers && !CheckValidationLayerSupport()) 
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
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

void DeviceContext::SetupDebugMessenger()
{
    if (!enableValidationLayers)
        return;

    DebugMessenger::InitializeDebugMessenger(instance, &debugMessenger);
}

void DeviceContext::CreateDevice()
{
    DeviceCreateInfo createInfo(instance, &surface, window);

    Device::InitalizeDevice(createInfo, &device);
}

void DeviceContext::CreateSurface()
{
    Surface::InitializeSurface(instance, window, &surface);
}

bool DeviceContext::CheckValidationLayerSupport() 
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

std::vector<const char*> DeviceContext::GetRequiredExtensions() 
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

Device* DeviceContext::GetDevice()
{
    return &device;
}

void DeviceContext::Cleanup()
{
    device.Cleanup();

    if (enableValidationLayers) 
    {
        debugMessenger.Cleanup();
    }

    surface.Cleanup();

    vkDestroyInstance(instance, nullptr);
    std::cout << "[Cleaned] Device Context" << std::endl;
}