#pragma once

#ifndef RENDERENGINE_GRAPHICSAPPLICATION
#define RENDERENGINE_GRAPHICSAPPLICATION

#include <GLFW/glfw3.h>

namespace RenderEngine
{
	class DeviceContext;
	class Window;

	class GraphicsApplication
	{
	private:
		Window* window;
		DeviceContext* deviceContext;

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