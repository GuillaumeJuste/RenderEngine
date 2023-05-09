#pragma once

#ifndef RENDERENGINE_SWAPCHAINCOMMANDBUFFERCREATEINFO
#define RENDERENGINE_SWAPCHAINCOMMANDBUFFERCREATEINFO

#include "Rendering/Base/DataStruct/ViewportData.hpp"
#include "Rendering/Base/DataStruct/ScissorData.hpp"

namespace RenderEngine::Rendering
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