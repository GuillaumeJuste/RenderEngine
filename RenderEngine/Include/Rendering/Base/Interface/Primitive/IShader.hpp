#pragma once

#ifndef RENDERENGINE_ISHADER
#define RENDERENGINE_ISHADER

namespace RenderEngine::Rendering
{
	class IShader
	{
	public:
		virtual void Clean() = 0;
	};
}

#endif