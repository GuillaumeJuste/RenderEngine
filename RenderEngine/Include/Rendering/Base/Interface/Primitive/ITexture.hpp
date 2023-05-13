#pragma once

#ifndef RENDERENGINE_ITEXTURE
#define RENDERENGINE_ITEXTURE

namespace RenderEngine::Rendering
{
	class ITexture
	{
	public:
		virtual void Clean() = 0;
	};
}

#endif