#pragma once

#ifndef RENDERENGINE_SWAPCHAINCOMMANDBUFFERCREATEINFO
#define RENDERENGINE_SWAPCHAINCOMMANDBUFFERCREATEINFO

#include "Engine/Base/DataStruct/ViewportData.hpp"
#include "Engine/Base/DataStruct/ScissorData.hpp"

namespace RenderEngine::Engine::Base
{
	struct SwapChainCommandBufferCreateInfo
	{
		bool customViewport = false;
		ViewportData viewportData;
		bool customScissor = false;
		ScissorData scissorData;
	};
}

#endif