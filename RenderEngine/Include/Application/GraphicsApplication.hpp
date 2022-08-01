#pragma once

#ifndef RENDERENGINE_GRAPHICSAPPLICATION
#define RENDERENGINE_GRAPHICSAPPLICATION

#include "Vulkan/VulkanBaseInclude.hpp"
#include "Engine/Device/DeviceContext.hpp"
#include "Engine/Window/Window.hpp"

namespace RenderEngine
{ 
	using namespace Vulkan;

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