#pragma once

#ifndef RENDERENGINE_DEBUGMESSENGER
#define RENDERENGINE_DEBUGMESSENGER

#include "Vulkan/VulkanBaseInclude.hpp"

namespace RenderEngine
{
	class DebugMessenger
	{
	private:
		VkDebugUtilsMessengerEXT debugMessenger;
		VkInstance instance;


		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT _messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData,
			void* _pUserData);
		
		VkResult CreateDebugUtilsMessengerEXT(
			const VkDebugUtilsMessengerCreateInfoEXT* _pCreateInfo,
			const VkAllocationCallbacks* _pAllocator,
			VkDebugUtilsMessengerEXT* _pDebugMessenger);

		void DestroyDebugUtilsMessengerEXT(
			VkDebugUtilsMessengerEXT _debugMessenger,
			const VkAllocationCallbacks* _pAllocator);
	public:
		DebugMessenger() = default;
		~DebugMessenger() = default;

		void InitializeDebugMessenger(const VkInstance& _instance);

		static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo);
		void Cleanup();
	};
}

#endif