#pragma once

#ifndef RENDERENGINE_IRENDERCONTEXT
#define RENDERENGINE_IRENDERCONTEXT

#include "Core/Scene/Scene.hpp"

namespace RenderEngine::Engine::Base
{
	class IRenderContext
	{
	public:
		virtual void DrawFrame() = 0;
		virtual void DrawScene(RenderEngine::Core::Scene* _scene) = 0;
	};
}

#endif