#pragma once

#ifndef RENDERENGINE_IRENDERCONTEXT
#define RENDERENGINE_IRENDERCONTEXT

#include "SceneGraph/Scene/Scene.hpp"
#include "ResourceManager/Assets/Mesh/RawMesh.hpp"
#include "ResourceManager/Assets/Mesh/Mesh.hpp"
#include "ResourceManager/Assets/Texture/RawTexture.hpp"
#include "ResourceManager/Assets/Texture/Texture.hpp"
#include "ResourceManager/Assets/Shader/RawShader.hpp"
#include "ResourceManager/Assets/Shader/Shader.hpp"

namespace RenderEngine::Rendering
{
	class IRenderContext
	{
	public:
		virtual void DrawScene(RenderEngine::SceneGraph::Scene* _scene) = 0;
		virtual bool CreateMesh(const RenderEngine::Assets::RawMesh& _input, RenderEngine::Assets::Mesh* _output) = 0;
		virtual bool CreateTexture(const RenderEngine::Assets::RawTexture& _input, RenderEngine::Assets::Texture* _output, bool _generateMipMap = true) = 0;
		virtual bool CreateShader(const RenderEngine::Assets::RawShader& _input, RenderEngine::Assets::Shader* _output) = 0;
		virtual bool CreateCubemap(ITexture* _texture, Mathlib::Vec2 _outputSize, bool _generateMipmap, RenderEngine::Assets::Texture* _output, 
			RenderEngine::Assets::Mesh* _mesh, RenderEngine::Assets::Shader* _vertexShader, RenderEngine::Assets::Shader* _fragmentShader) = 0;
		virtual bool CreatePrefilteredCubemap(ITexture* _texture, Mathlib::Vec2 _outputSize, RenderEngine::Assets::Texture* _output,
			RenderEngine::Assets::Mesh* _mesh, RenderEngine::Assets::Shader* _vertexShader, RenderEngine::Assets::Shader* _fragmentShader) = 0;

		virtual std::vector<char> GetTextureContent(RenderEngine::Assets::Texture* _texture, uint32_t _imageTotalSize) = 0;
	};
}

#endif