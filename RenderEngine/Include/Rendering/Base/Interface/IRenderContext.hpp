#pragma once

#ifndef RENDERENGINE_IRENDERCONTEXT
#define RENDERENGINE_IRENDERCONTEXT

#include "SceneGraph/Scene/Scene.hpp"
#include "AssetLoader/RawAsset/Mesh/RawMesh.hpp"
#include "AssetLoader/RawAsset/Texture/RawTexture.hpp"
#include "AssetLoader/RawAsset/Shader/RawShader.hpp"

#include "ResourceManager/Assets/Mesh/Mesh.hpp"
#include "ResourceManager/Assets/Texture/Texture.hpp"
#include "ResourceManager/Assets/Shader/Shader.hpp"

namespace RenderEngine::Rendering
{
	class IRenderContext
	{
	public:
		virtual void DrawScene(RenderEngine::SceneGraph::Scene* _scene) = 0;
		virtual bool CreateMesh(const Loader::RawMesh& _input, RenderEngine::Assets::Mesh* _output) = 0;
		virtual bool CreateTexture(const Loader::RawTexture& _input, RenderEngine::Assets::Texture* _output, bool _generateMipMap = true) = 0;
		virtual bool CreateShader(const Loader::RawShader& _input, RenderEngine::Assets::Shader* _output) = 0;
		virtual bool CreateCubemap(ITexture* _texture, Mathlib::Vec2 _outputSize, bool _generateMipmap, RenderEngine::Assets::Texture* _output, 
			RenderEngine::Assets::Mesh* _mesh, RenderEngine::Assets::Shader* _vertexShader, RenderEngine::Assets::Shader* _fragmentShader) = 0;
		virtual bool CreatePrefilteredCubemap(ITexture* _texture, Mathlib::Vec2 _outputSize, RenderEngine::Assets::Texture* _output,
			RenderEngine::Assets::Mesh* _mesh, RenderEngine::Assets::Shader* _vertexShader, RenderEngine::Assets::Shader* _fragmentShader) = 0;

		virtual std::vector<char> GetTextureContent(RenderEngine::Assets::Texture* _texture, uint32_t _imageTotalSize) = 0;
	};
}

#endif