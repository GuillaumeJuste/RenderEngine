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
        GameObject* gameObject = nullptr;
    public:
        /**
         * @brief should the object be displayed
        */
        bool enable = true;

        Component() = default;
        ~Component() = default;

        virtual void Initialize() = 0;
        virtual void Start() = 0;
        virtual void Update(double _deltaTime) = 0;
        virtual void FixedUpdate(double _deltaTime) = 0;

        friend class GameObject;
    };
}

#endif