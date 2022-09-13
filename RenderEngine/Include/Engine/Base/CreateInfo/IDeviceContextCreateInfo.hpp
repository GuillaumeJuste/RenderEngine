#pragma once

#ifndef RENDERENGINE_IDEVICECONTEXTCREATEINFO
#define RENDERENGINE_IDEVICECONTEXTCREATEINFO

#include "Window/Glfw/Window.hpp"

using namespace RenderEngine::Window;

namespace RenderEngine::Engine::Base
{
	struct IDeviceContextCreateInfo
	{
		/// window
		GLFW::Window* window;
	};
}

#endif