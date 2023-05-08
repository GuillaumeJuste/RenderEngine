#pragma once

#ifndef RENDERENGINE_MESHDATA
#define RENDERENGINE_MESHDATA

#include "Engine/Vulkan/BufferObject/BufferObject.hpp"
#include "ResourceManager/Assets/Mesh/Mesh.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct MeshData
	{
		RenderEngine::Assets::Mesh* mesh = nullptr;

		BufferObject vertexBufferObject;
		BufferObject indexBufferObject;

		/// default constructor
		MeshData() = default;

		bool operator==(RenderEngine::Assets::Mesh* _mesh) const;

		void Cleanup();
	};
}

#endif