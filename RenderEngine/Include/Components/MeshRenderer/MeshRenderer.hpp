#pragma once

#ifndef RENDERENGINE_MESHRENDERER
#define RENDERENGINE_MESHRENDERER

#include "SceneGraph/Components/Component.hpp"
#include "ResourceManager/Assets/Mesh/Mesh.hpp"
#include "ResourceManager/Assets/Shader/Shader.hpp"
#include "Rendering/Base/Enum/PolygonDrawMode.hpp"
#include "Rendering/Base/Enum/FrontFace.hpp"
#include "Components/MeshRenderer/Material.hpp"

using namespace RenderEngine::Assets;
using namespace RenderEngine::Rendering;

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

        /// Vertex Shader
        Shader* vertexShader = nullptr;
        
        /// Fragment Shader
        Shader* fragmentShader = nullptr;
        
        /// vertex draw order
        FrontFace frontFace = FrontFace::CLOCKWISE;

        /// mesh draw mode
        PolygonDrawMode drawMode = PolygonDrawMode::FILL;

        /// outline width
        float lineWidth = 1.0f;

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