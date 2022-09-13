#pragma once

#ifndef RENDERENGINE_IDEVICECONTEXT
#define RENDERENGINE_IDEVICECONTEXT

#include "Engine/Base/CreateInfo/IDeviceContextCreateInfo.hpp"
#include "Engine/Base/CreateInfo/IRenderContextCreateInfo.hpp"
#include "Engine/Base/Interface/IRenderContext.hpp"


namespace RenderEngine::Engine::Base
{
	class IDeviceContext
	{
	public:
		virtual IRenderContext* CreateRenderContext(const IRenderContextCreateInfo& _createInfo) = 0;
		virtual void WaitDeviceIdle() = 0;
	};
}

#endif