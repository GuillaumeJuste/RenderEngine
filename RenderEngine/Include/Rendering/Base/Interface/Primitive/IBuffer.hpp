#pragma once

#ifndef RENDERENGINE_IBUFFER
#define RENDERENGINE_IBUFFER

namespace RenderEngine::Rendering
{
	class IBuffer
	{
	public:
		virtual void Clean() {};
	};
}

#endif