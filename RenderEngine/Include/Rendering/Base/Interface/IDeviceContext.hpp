#pragma once

#ifndef RENDERENGINE_IDEVICECONTEXT
#define RENDERENGINE_IDEVICECONTEXT

#include "Rendering/Base/CreateInfo/IDeviceContextCreateInfo.hpp"
#include "Rendering/Base/CreateInfo/IRenderContextCreateInfo.hpp"
#include "Rendering/Base/Interface/IRenderContext.hpp"

#include <vector>

namespace RenderEngine::Rendering
{
	class IDeviceContext
	{
	public:
		virtual void InitializeDeviceContext(std::string _physicalDeviceName) = 0;
		virtual IRenderContext* CreateRenderContext(const IRenderContextCreateInfo& _createInfo) = 0;
		virtual std::vector<std::string> QueryAvailblePhysicalDevices() = 0;
		virtual void WaitDeviceIdle() = 0;
	};
}

#endif