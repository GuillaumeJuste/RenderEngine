#pragma once

#ifndef RENDERENGINE_DEBUGMESSENGER
#define RENDERENGINE_DEBUGMESSENGER

#include "Vulkan/Misc/VulkanBaseInclude.hpp"

namespace RenderEngine::Vulkan
{
	/**
	 * @brief Class to manage creation and usage of Vulkan Debug Messenger
	*/
	class DebugMessenger
	{
	private:
		/// vulkan debug messenger handle
		VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
		
		/// vulkan instance
		VkInstance instance = VK_NULL_HANDLE;

		/**
		 * @brief function called by debug messenger
		 * @param _messageSeverity message serverty
		 * @param _messageType mssage type
		 * @param _pCallbackData callback data
		 * @param _pUserData user data
		 * @return VK_FALSE
		*/
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT _messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData,
			void* _pUserData);
		
		/**
		 * @brief Initialize debug messenger
		 * @param _pCreateInfo debug messenger creation info
		 * @param _pAllocator allocation callback
		 * @return true if debug messenger is Initialized
		*/
		VkResult CreateDebugUtilsMessengerEXT(
			const VkDebugUtilsMessengerCreateInfoEXT* _pCreateInfo,
			const VkAllocationCallbacks* _pAllocator);

		/**
		 * @brief Destroy debug messenger
		 * @param _debugMessenger debug messenger to destroy
		 * @param _pAllocator allocation callback
		*/
		void DestroyDebugUtilsMessengerEXT(
			VkDebugUtilsMessengerEXT _debugMessenger,
			const VkAllocationCallbacks* _pAllocator);
	public:

		/// default constructor
		DebugMessenger() = default;
		
		/// default destructor
		~DebugMessenger() = default;

		/**
		 * @brief Initialize debug callback
		 * @param _instance vulkan instance
		 * @param _output Debug Messenger to allocate
		*/
		static void InitializeDebugMessenger(const VkInstance& _instance, DebugMessenger* _output);

		/**
		 * @brief Fill debug messenger create info
		 * @param _createInfo create info to fill
		*/
		static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo);
		
		/**
		 * @brief cleanup vulkan classes
		*/
		void Cleanup();
	};
}

#endif