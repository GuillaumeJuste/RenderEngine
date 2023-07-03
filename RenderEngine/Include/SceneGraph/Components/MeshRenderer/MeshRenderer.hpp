#pragma once

#ifndef RENDERENGINE_MESHRENDERER
#define RENDERENGINE_MESHRENDERER

#include "SceneGraph/Components/Component.hpp"
#include "ResourceManager/Assets/Mesh/Mesh.hpp"
#include "ResourceManager/Assets/Shader/Shader.hpp"
#include "Rendering/Base/Enum/PolygonDrawMode.hpp"
#include "Rendering/Base/Enum/FrontFace.hpp"
#include "SceneGraph/Components/MeshRenderer/Material.hpp"

using namespace RenderEngine::Assets;
using namespace RenderEngine::Rendering;

namespace RenderEngine::SceneGraph
{
    class MeshRenderer : public Component
    {
    public:

        Mesh* mesh;
        Material material;

        Shader* vertexShader;
        Shader* fragmentShader;
        FrontFace frontFace = FrontFace::CLOCKWISE;
        PolygonDrawMode drawMode = PolygonDrawMode::FILL;
        float lineWidth = 1.0f;

        MeshRenderer() = default;
        ~MeshRenderer() = default;

        void Initialize();
        void Start();
        void Update();
    };
}

#endif