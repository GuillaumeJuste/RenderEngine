#pragma once

#ifndef RENDERENGINE_FRONTFACE
#define RENDERENGINE_FRONTFACE

#include <string>

namespace RenderEngine::Engine::Base
{
	enum class FrontFace
	{
		COUNTER_CLOCKWISE = 0,
		CLOCKWISE = 1
	};
}

#endif