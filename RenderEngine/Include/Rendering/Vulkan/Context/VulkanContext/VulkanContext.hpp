#pragma once

#ifndef RENDERENGINE_VULKANCONTEXT
#define RENDERENGINE_VULKANCONTEXT

#include "Rendering/Base/Interface/IEngineInstance.hpp"
#include "Rendering/Base/Interface/IDeviceContext.hpp"

#include "Rendering/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Rendering/Vulkan/Context/DeviceContext/DeviceContext.hpp"
#include "Rendering/Vulkan/Debugger/DebugMessenger.hpp"
#include "Rendering/Vulkan/WindowProperties/WindowProperties.hpp"

#include <forward_list>

using namespace RenderEngine::Window;

namespace RenderEngine::Rendering
{
	/**
	 * @brief Class to manage creation and usage of a Vulkan instance
	*/
	class VulkanContext : public IEngineInstance
	{
	private:
		/**
		 * @brief Vulkan instance
		*/
		VkInstance instance = VK_NULL_HANDLE;
		
		/**
		 * @brief Debug messenger used to catch and display Vulkan errors
		*/
		DebugMessenger debugMessenger;
		
		/**
		 * @brief list of devices created by the instance
		*/
		std::forward_list<DeviceContext> deviceContexts;

		/**
		 * @brief List of pairs of Window and surface created via this vulkan instance
		*/
		std::forward_list<WindowProperties> windowsProperties;

		/**
		 * @brief create the vulkan instance 
		 * @param _createinfo instance initialisation parameters
		*/
		void CreateInstance(const IEngineInstanceCreateInfo& _createinfo);

		/**
		 * @brief Setup debug messenger to catch vulakn errors
		*/
		void SetupDebugMessenger();


		/**
		 * @brief check support of validation layer in the list of supported layers
		 * @return true if validation layer is supported
		*/
		bool CheckValidationLayerSupport();

		/**
		 * @brief Get a vector of all the requiered extentions to create the vulkan instance
		*/
		std::vector<const char*> GetRequiredExtensions();

		/**
		 * @brief Add a window to the list of window supported by this vulkan instance
		 * @param _window window to add to the list
		 * @return
		*/
		WindowProperties* AddWindow(IWindow* _window);

	public:

		///default constructor
		VulkanContext() = default;

		///default destructor
		~VulkanContext() = default;

		/**
		 * @brief Initialize vulkan instance
		 * @param _createinfo instance initialisation parameters
		*/
		void InitializeInstance(const IEngineInstanceCreateInfo& _createinfo);

		/**
		 * @brief create a Device context from this instance
		 * @param _createinfo device context initialisation parameters
		 * @return pointer to the created device context
		*/
		IDeviceContext* CreateDeviceContext(const IDeviceContextCreateInfo& _createinfo);

		/**
		 * @brief clean the stored DeviceContexts and WindowProperties then clean the vulkan instance
		*/
		void Cleanup();
	};
}

#endif