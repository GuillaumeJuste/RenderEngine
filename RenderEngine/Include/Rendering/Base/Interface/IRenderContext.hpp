#pragma once

#ifndef RENDERENGINE_IRENDERCONTEXT
#define RENDERENGINE_IRENDERCONTEXT

#include "Rendering/Base/ResourceManager/ResourceManager.hpp"

#include "SceneGraph/Scene/Scene.hpp"

namespace RenderEngine::Rendering
{
	class IRenderContext
	{
	public:
		virtual void DrawScene(RenderEngine::SceneGraph::Scene* _scene) = 0;
		virtual ResourceManager* CreateResourceManager() = 0;
	};
}

#endif