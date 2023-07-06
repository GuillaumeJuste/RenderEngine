#pragma once

#ifndef RENDERENGINE_IDEVICECONTEXTCREATEINFO
#define RENDERENGINE_IDEVICECONTEXTCREATEINFO

#include "Window/WindowBase.hpp"

using namespace RenderEngine::Window;

namespace RenderEngine::Rendering
{
	struct IDeviceContextCreateInfo
	{
		/// window
		WindowBase* window;
	};
}

#endif