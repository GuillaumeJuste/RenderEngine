#pragma once

#ifndef RENDERENGINE_IRENDERCONTEXT
#define RENDERENGINE_IRENDERCONTEXT

#include "SceneGraph/Scene/Scene.hpp"
#include "ResourceManager/Assets/Mesh/RawMesh.hpp"
#include "ResourceManager/Assets/Mesh/Mesh.hpp"
#include "ResourceManager/Assets/Texture/RawTexture.hpp"
#include "ResourceManager/Assets/Texture/Texture.hpp"

namespace RenderEngine::Rendering
{
	class IRenderContext
	{
	public:
		virtual void DrawScene(RenderEngine::SceneGraph::Scene* _scene) = 0;
		virtual bool CreateMesh(const RenderEngine::Assets::RawMesh& _input, RenderEngine::Assets::Mesh* _output) = 0;
		virtual bool CreateTexture(const RenderEngine::Assets::RawTexture& _input, RenderEngine::Assets::Texture* _output) = 0;
	};
}

#endif