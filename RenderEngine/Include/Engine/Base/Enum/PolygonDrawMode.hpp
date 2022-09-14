#pragma once

#ifndef RENDERENGINE_POLYGONDRAWMODE
#define RENDERENGINE_POLYGONDRAWMODE

#include <string>

namespace RenderEngine::Engine::Base
{
	enum class PolygonDrawMode
	{
		FILL = 0,
		LINE = 1,
		POINT = 2 
	};
}

#endif