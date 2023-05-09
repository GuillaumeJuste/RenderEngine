#pragma once

#ifndef RENDERENGINE_MESHRENDERER
#define RENDERENGINE_MESHRENDERER

#include "SceneGraph/Components/Component.hpp"
#include "ResourceManager/Assets/Mesh/Mesh.hpp"
#include "ResourceManager/Assets/Texture/Texture.hpp"
#include "Rendering/Base/Enum/PolygonDrawMode.hpp"
#include "Rendering/Base/Enum/FrontFace.hpp"

using namespace RenderEngine::Assets;
using namespace RenderEngine::Rendering;

namespace RenderEngine::SceneGraph
{
    class MeshRenderer : public Component
    {
    public:

        Mesh* mesh;
        Texture* texture;
        Texture* metalnessMap;
        Texture* roughnessMap;
        Texture* ambientOcclusionMap;

        std::string vertexShaderFilePath;
        std::string fragmentShaderFilePath;
        FrontFace frontFace = FrontFace::CLOCKWISE;
        PolygonDrawMode drawMode = PolygonDrawMode::FILL;
        float lineWidth = 1.0f;
        float shininess;
        Mathlib::Vec3 ambient;
        Mathlib::Vec3 diffuse;
        Mathlib::Vec3 specular;

        MeshRenderer() = default;
        ~MeshRenderer() = default;

        void Initialize();
        void Start();
        void Update();
    };
}

#endif