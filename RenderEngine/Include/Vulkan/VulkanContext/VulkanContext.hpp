#pragma once

#ifndef RENDERENGINE_VULKANCONTEXT
#define RENDERENGINE_VULKANCONTEXT

#include "Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Vulkan/DeviceContext/DeviceContext.hpp"
#include "Vulkan/Surface/Surface.hpp"
#include "Vulkan/Debugger/DebugMessenger.hpp"
#include "Glfw/Window/Window.hpp"

namespace RenderEngine::Vulkan
{
	class VulkanContext
	{
	private:
		VkInstance instance;
		Window* window;

		DebugMessenger debugMessenger;
		DeviceContext device;

		void CreateInstance();
		void SetupDebugMessenger();
		void CreateDevice();

		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
	public:
		VulkanContext(Window* _window);
		~VulkanContext() = default;

		DeviceContext* GetDeviceContext();

		void Cleanup();
	};
}

#endif