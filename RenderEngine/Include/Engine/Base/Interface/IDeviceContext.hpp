#pragma once

#ifndef RENDERENGINE_IDEVICECONTEXT
#define RENDERENGINE_IDEVICECONTEXT

#include "Engine/Base/CreateInfo/IDeviceContextCreateInfo.hpp"
#include "Engine/Base/CreateInfo/IRenderContextCreateInfo.hpp"
#include "Engine/Base/Interface/IRenderContext.hpp"

#include <vector>

namespace RenderEngine::Engine::Base
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