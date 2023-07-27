#pragma once

#ifndef RENDERENGINE_MESHRENDERER
#define RENDERENGINE_MESHRENDERER

#include "SceneGraph/Components/Component.hpp"
#include "ResourceManager/Assets/Mesh/Mesh.hpp"
#include "Components/MeshRenderer/Material.hpp"

using namespace RenderEngine::Assets;

namespace RenderEngine::Component
{
    /**
    * @brief MeshRenderer class implementation
   */
    class MeshRenderer : public RenderEngine::SceneGraph::Component
    {
    public:

        /// GameObject mesh
        Mesh* mesh = nullptr;
        
        /// GameObject material
        Material material;

        /// default constructor
        MeshRenderer() = default;

        /// default destructor
        ~MeshRenderer() = default;

        /// Component Initiliazation
        void Initialize();

        /// Component Start
        void Start();

        /**
         * @brief Component Update
         * @param _deltaTime frame delta time
        */
        void Update(double _deltaTime);

        /**
         * @brief Component Fixed Update
         * @param _deltaTime fixed delta time
        */
        void FixedUpdate(double _deltaTime);
    };
}

#endif