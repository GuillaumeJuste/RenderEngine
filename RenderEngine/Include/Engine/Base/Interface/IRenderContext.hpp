#pragma once

#ifndef RENDERENGINE_IRENDERCONTEXT
#define RENDERENGINE_IRENDERCONTEXT

#include "SceneGraph/Scene/Scene.hpp"

namespace RenderEngine::Engine::Base
{
	class IRenderContext
	{
	public:
		virtual void DrawScene(RenderEngine::SceneGraph::Scene* _scene) = 0;
	};
}

#endif