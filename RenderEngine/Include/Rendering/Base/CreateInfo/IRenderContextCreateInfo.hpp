#pragma once

#ifndef RENDERENGINE_IRENDERCONTEXTCREATEINFO
#define RENDERENGINE_IRENDERCONTEXTCREATEINFO

#include "Rendering/Base/CreateInfo/SwapChainCommandBufferCreateInfo.hpp"

namespace RenderEngine::Rendering
{
	struct IRenderContextCreateInfo
	{
		SwapChainCommandBufferCreateInfo* swapChainCommandBufferCreateInfo = nullptr;
	};
}

#endif