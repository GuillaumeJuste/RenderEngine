#pragma once

#ifndef RENDERENGINE_MESHDATA
#define RENDERENGINE_MESHDATA

#include "Engine/Vulkan/BufferObject/BufferObject.hpp"
#include "Core/RessourceManager/Mesh.hpp"

namespace RenderEngine::Engine::Vulkan
{
	struct MeshData
	{
		RenderEngine::Core::Mesh* mesh;

		BufferObject vertexBufferObject;
		BufferObject indexBufferObject;

		/// default constructor
		MeshData() = default;

		bool operator==(RenderEngine::Core::Mesh* _mesh) const;

		void Cleanup();
	};
}

#endif