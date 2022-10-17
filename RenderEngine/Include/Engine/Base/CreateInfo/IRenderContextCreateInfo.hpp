#pragma once

#ifndef RENDERENGINE_IRENDERCONTEXTCREATEINFO
#define RENDERENGINE_IRENDERCONTEXTCREATEINFO

#include "Engine/Base/CreateInfo/SwapChainCommandBufferCreateInfo.hpp"

namespace RenderEngine::Engine::Base
{
	struct IRenderContextCreateInfo
	{
		SwapChainCommandBufferCreateInfo* swapChainCommandBufferCreateInfo = nullptr;
	};
}

#endif