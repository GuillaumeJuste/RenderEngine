#pragma once

#ifndef RENDERENGINE_DEVICECONTEXT
#define RENDERENGINE_DEVICECONTEXT

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Device.hpp"
#include "Engine/Surface.hpp"
#include "Engine/DebugMessenger.hpp"

namespace RenderEngine
{
	class DebugMessenger;
	class Window;

	class DeviceContext
	{
	private:
		VkInstance instance;
		Window* window;

		DebugMessenger debugMessenger;
		Device device;
		Surface surface;

		void CreateInstance();
		void SetupDebugMessenger();
		void CreateDevice();
		void CreateSurface();

		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
	public:
		DeviceContext(Window* _window);
		~DeviceContext() = default;

		void Cleanup();
	};
}

#endif