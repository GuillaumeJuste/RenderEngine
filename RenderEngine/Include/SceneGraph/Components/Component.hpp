#pragma once

#ifndef RENDERENGINE_COMPONENT
#define RENDERENGINE_COMPONENT

#include "SceneGraph/Object/Object.hpp"

namespace RenderEngine::SceneGraph
{
    class GameObject;

    /**
     * @brief Component class implementation
    */
    class Component
    {
    protected :
        /// owning GameObject
        GameObject* gameObject = nullptr;
    public:
        /**
         * @brief should the object be displayed
        */
        bool enable = true;

        /// default constructor
        Component() = default;

        /// default destructor
        ~Component() = default;

        /**
         * @brief Component Initiliazation
        */
        virtual void Initialize() = 0;

        /**
         * @brief Component Start
        */
        virtual void Start() = 0;

        /**
         * @brief Component Update
         * @param _deltaTime frame delta time
        */
        virtual void Update(double _deltaTime) = 0;

        /**
         * @brief Component Fixed Update
         * @param _deltaTime fixed delta time
        */
        virtual void FixedUpdate(double _deltaTime) = 0;

        friend class GameObject;
    };
}

#endif