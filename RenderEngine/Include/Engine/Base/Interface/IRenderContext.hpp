#pragma once

#ifndef RENDERENGINE_IRENDERCONTEXT
#define RENDERENGINE_IRENDERCONTEXT

namespace RenderEngine::Engine::Base
{
	class IRenderContext
	{
	public:
		virtual void DrawFrame() = 0;
	};
}

#endif