#pragma once

#ifndef RENDERENGINE_MESHRENDERER
#define RENDERENGINE_MESHRENDERER

#include "Core/Object/Components/Component.hpp"
#include "Core/Object/Components/MeshRenderer/Mesh.hpp"

namespace RenderEngine::Core
{
    /**
     * @brief Component class implementation
    */
    class MeshRenderer : public Component
    {
    private:
        Mesh mesh;
    public:
        MeshRenderer() = default;
        ~MeshRenderer() = default;

        void Initialize();
        void Start();
        void Update();

        Mesh* GetMesh();
        // TODO : change to pointer when resourceManager is implemented
        void SetMesh(const Mesh& _mesh);
    };
}

#endif