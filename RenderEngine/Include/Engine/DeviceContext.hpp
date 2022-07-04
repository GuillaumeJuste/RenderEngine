#pragma once

#ifndef RENDERENGINE_DEVICECONTEXT
#define RENDERENGINE_DEVICECONTEXT

#include <GLFW/glfw3.h>
#include <vector>

#include "Engine/Device.hpp"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

namespace RenderEngine
{
	class DebugMessenger;

	class DeviceContext
	{
	private:
		VkInstance instance;
		DebugMessenger* debugMessenger;

		Device device;

		void CreateInstance();
		void SetupDebugMessenger();
		void CreateDevice();

		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
	public:
		DeviceContext();
		~DeviceContext() = default;

		void Cleanup();
	};
}

#endif