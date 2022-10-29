#pragma once

#ifndef RENDERENGINE_MESHRENDERER
#define RENDERENGINE_MESHRENDERER

#include "Core/Components/Component.hpp"
#include "Core/RessourceManager/Mesh.hpp"
#include "Core/RessourceManager/Texture.hpp"
#include "Engine/Base/Enum/PolygonDrawMode.hpp"
#include "Engine/Base/Enum/FrontFace.hpp"

using namespace RenderEngine::Engine::Base;

namespace RenderEngine::Core
{
    class MeshRenderer : public Component
    {
    private:
        Mesh* mesh;
        Texture* texture;

    public:

        std::string vertexShaderFilePath;
        std::string fragmentShaderFilePath;
        FrontFace frontFace = FrontFace::CLOCKWISE;
        PolygonDrawMode drawMode = PolygonDrawMode::FILL;
        float lineWidth = 1.0f;


        MeshRenderer() = default;
        ~MeshRenderer() = default;

        void Initialize();
        void Start();
        void Update();

        Mesh* GetMesh();
        void SetMesh(Mesh* _mesh);

        Texture* GetTexture();
        void SetTexture(Texture* _texture);
    };
}

#endif