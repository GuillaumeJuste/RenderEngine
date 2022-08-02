#pragma once

#ifndef RENDERENGINE_VULKANCONTEXT
#define RENDERENGINE_VULKANCONTEXT

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/DeviceContext/DeviceContext.hpp"
#include "Engine/Surface/Surface.hpp"
#include "Engine/Debugger/DebugMessenger.hpp"
#include "Engine/Window/Window.hpp"

namespace RenderEngine::Vulkan
{
	class VulkanContext
	{
	private:
		VkInstance instance;
		Window* window;

		DebugMessenger debugMessenger;
		DeviceContext device;
		Surface surface;

		void CreateInstance();
		void SetupDebugMessenger();
		void CreateDevice();
		void CreateSurface();

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