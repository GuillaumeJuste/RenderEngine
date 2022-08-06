#pragma once

#ifndef RENDERENGINE_DEBUGMESSENGER
#define RENDERENGINE_DEBUGMESSENGER

#include "Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Vulkan
{
	class DebugMessenger
	{
	private:
		VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
		VkInstance instance = VK_NULL_HANDLE;


		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT _messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData,
			void* _pUserData);
		
		VkResult CreateDebugUtilsMessengerEXT(
			const VkDebugUtilsMessengerCreateInfoEXT* _pCreateInfo,
			const VkAllocationCallbacks* _pAllocator);

		void DestroyDebugUtilsMessengerEXT(
			VkDebugUtilsMessengerEXT _debugMessenger,
			const VkAllocationCallbacks* _pAllocator);
	public:
		DebugMessenger() = default;
		~DebugMessenger() = default;

		static void InitializeDebugMessenger(const VkInstance& _instance, DebugMessenger* _output);

		static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo);
		void Cleanup();
	};
}

#endif