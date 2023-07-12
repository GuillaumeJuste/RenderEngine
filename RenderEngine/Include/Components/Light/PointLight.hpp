#pragma once

#ifndef RENDERENGINE_POINTLIGHT
#define RENDERENGINE_POINTLIGHT

#include "SceneGraph/Components/Component.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"
#include "Components/Light/Light.hpp"

namespace RenderEngine::Component
{
    /**
     * @brief Point Light class implementation
    */
    class PointLight : public RenderEngine::SceneGraph::Component, public Light
    {
    public:
        /// light range
        float range;

        /// default constructor
        PointLight() = default;

        /// default destructor
        ~PointLight() = default;

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