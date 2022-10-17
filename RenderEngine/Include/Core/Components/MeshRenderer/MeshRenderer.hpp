#pragma once

#ifndef RENDERENGINE_MESHRENDERER
#define RENDERENGINE_MESHRENDERER

#include "Core/Components/Component.hpp"
#include "Core/RessourceManager/Mesh.hpp"
#include "Core/RessourceManager/Texture.hpp"

namespace RenderEngine::Core
{
    class MeshRenderer : public Component
    {
    private:
        Mesh* mesh;
        Texture* texture;

    public:
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