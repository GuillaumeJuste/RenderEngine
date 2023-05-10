#include <iostream>

#include "Rendering/Vulkan/Debugger/DebugMessenger.hpp"

using namespace RenderEngine::Rendering;

void DebugMessenger::InitializeDebugMessenger(const VkInstance& _instance, DebugMessenger* _output)
{
    _output->instance = _instance;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    PopulateDebugMessengerCreateInfo(createInfo);

    if (_output->CreateDebugUtilsMessengerEXT(&createInfo, nullptr) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

void DebugMessenger::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo)
{
    _createInfo = {};
    _createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    _createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    _createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    _createInfo.pfnUserCallback = DebugCallback;
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessenger::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT _messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData,
    void* _pUserData)
{
    if (_messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        std::cerr << "validation layer: " << _pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }
    return VK_FALSE;
}

VkResult DebugMessenger::CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* _pCreateInfo, const VkAllocationCallbacks* _pAllocator) 
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, _pCreateInfo, _pAllocator, &debugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DebugMessenger::DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT _debugMessenger, const VkAllocationCallbacks* _pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, _debugMessenger, _pAllocator);
    }
}

void DebugMessenger::Cleanup()
{
    DestroyDebugUtilsMessengerEXT(debugMessenger, nullptr);

    std::cout << "[Cleaned] Device Debug Messenger" << std::endl;
}