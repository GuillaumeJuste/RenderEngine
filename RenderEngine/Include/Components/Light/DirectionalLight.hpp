#pragma once

#ifndef RENDERENGINE_DIRECTIONALLIGHT
#define RENDERENGINE_DIRECTIONALLIGHT

#include "SceneGraph/Components/Component.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"
#include "Components/Light/Light.hpp"

namespace RenderEngine::Component
{
    /**
     * @brief Directional Light class implementation
    */
    class DirectionalLight : public RenderEngine::SceneGraph::Component, public Light
    {
    public:
        /// default constructor
        DirectionalLight() = default;

        /// default destructor
        ~DirectionalLight() = default;

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