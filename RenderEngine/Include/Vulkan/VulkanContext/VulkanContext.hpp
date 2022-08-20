#pragma once

#ifndef RENDERENGINE_VULKANCONTEXT
#define RENDERENGINE_VULKANCONTEXT

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/DeviceContext/DeviceContext.hpp"
#include "Vulkan/Surface/Surface.hpp"
#include "Vulkan/Debugger/DebugMessenger.hpp"
#include "Vulkan/WindowProperties/WindowProperties.hpp"
#include "Glfw/Window/Window.hpp"

#include <forward_list>

namespace RenderEngine::Vulkan
{
	class VulkanContext
	{
	private:
		VkInstance instance;

		DebugMessenger debugMessenger;
		std::forward_list<DeviceContext> deviceContexts;
		DeviceContext deviceContext;
		std::forward_list<WindowProperties> windowProperties;

		void CreateInstance();
		void SetupDebugMessenger();

		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
	public:
		VulkanContext();
		~VulkanContext() = default;

		WindowProperties* AddWindow(Window* _window);
		DeviceContext* CreateDeviceContext(WindowProperties* _windowProperties);

		void Cleanup();
	};
}

#endif