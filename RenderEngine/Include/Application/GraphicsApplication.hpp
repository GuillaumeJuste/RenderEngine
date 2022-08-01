#pragma once

#ifndef RENDERENGINE_GRAPHICSAPPLICATION
#define RENDERENGINE_GRAPHICSAPPLICATION

#include "Vulkan/VulkanBaseInclude.hpp"

namespace RenderEngine
{ 
	namespace Vulkan
	{
		class DeviceContext;
		class Window;
	}
	class GraphicsApplication
	{
	private:
		Vulkan::Window* window;
		Vulkan::DeviceContext* deviceContext;

		void InitVulkan();
		void InitWindow();
		void MainLoop();
		void Cleanup();


	public:
		GraphicsApplication() = default;
		~GraphicsApplication() = default;

		void Run();
	};
}

#endif