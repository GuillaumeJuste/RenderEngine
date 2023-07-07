#pragma once

#ifndef RENDERENGINE_IDEVICECONTEXTCREATEINFO
#define RENDERENGINE_IDEVICECONTEXTCREATEINFO

#include "Window/IWindow.hpp"

using namespace RenderEngine::Window;

namespace RenderEngine::Rendering
{
	struct IDeviceContextCreateInfo
	{
		/// window
		IWindow* window;
	};
}

#endif