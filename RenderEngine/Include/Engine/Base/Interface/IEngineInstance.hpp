#pragma once

#ifndef RENDERENGINE_IENGINEINSTANCE
#define RENDERENGINE_IENGINEINSTANCE

#include "Engine/Base/CreateInfo/IEngineInstanceCreateInfo.hpp"
#include "Engine/Base/CreateInfo/IDeviceContextCreateInfo.hpp"
#include "Engine/Base/Interface/IDeviceContext.hpp"

namespace RenderEngine::Engine::Base
{
	class IEngineInstance
	{
	public :
		virtual void InitializeInstance(const IEngineInstanceCreateInfo& _createinfo) = 0;
		virtual IDeviceContext* CreateDeviceContext(const IDeviceContextCreateInfo& _createinfo) = 0;
	};
}

#endif