#pragma once

#ifndef RENDERENGINE_SPOTLIGHT
#define RENDERENGINE_SPOTLIGHT

#include "SceneGraph/Components/Component.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"
#include "Components/Light/Light.hpp"

namespace RenderEngine::Component
{
    /**
     * @brief Spot Light class implementation
    */
    class SpotLight : public RenderEngine::SceneGraph::Component, public Light
    {
    public:
        /// Light range
        float range;

        /// light cut off angle in degree
        float cutOff;

        /// default constructor
        SpotLight() = default;

        /// default destructor
        ~SpotLight() = default;

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