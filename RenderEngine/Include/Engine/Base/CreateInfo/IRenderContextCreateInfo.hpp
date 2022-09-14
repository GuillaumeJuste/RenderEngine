#pragma once

#ifndef RENDERENGINE_IRENDERCONTEXTCREATEINFO
#define RENDERENGINE_IRENDERCONTEXTCREATEINFO

#include "Engine/Base/CreateInfo/GraphicsPipelineCreateInfo.hpp"
#include "Engine/Base/CreateInfo/SwapChainCommandBufferCreateInfo.hpp"

namespace RenderEngine::Engine::Base
{
	struct IRenderContextCreateInfo
	{
		GraphicsPipelineCreateInfo* graphicsPipelineCreateInfo = nullptr;
		SwapChainCommandBufferCreateInfo* swapChainCommandBufferCreateInfo = nullptr;
	};
}

#endif