#pragma once

#ifndef RENDERENGINE_IENGINEINSTANCE
#define RENDERENGINE_IENGINEINSTANCE

#include "Rendering/Base/CreateInfo/IEngineInstanceCreateInfo.hpp"
#include "Rendering/Base/CreateInfo/IDeviceContextCreateInfo.hpp"
#include "Rendering/Base/Interface/IDeviceContext.hpp"

namespace RenderEngine::Rendering
{
	class IEngineInstance
	{
	public :
		virtual void InitializeInstance(const IEngineInstanceCreateInfo& _createinfo) = 0;
		virtual IDeviceContext* CreateDeviceContext(const IDeviceContextCreateInfo& _createinfo) = 0;
	};
}

#endif