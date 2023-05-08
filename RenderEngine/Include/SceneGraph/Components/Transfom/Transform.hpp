#pragma once

#ifndef RENDERENGINE_TRANSFORM
#define RENDERENGINE_TRANSFORM

#include "SceneGraph/Components/Component.hpp"
#include "Transform/Transform.hpp"

namespace RenderEngine::SceneGraph
{
    /**
     * @brief Component class implementation
    */
    class Transform : public Component
    {
    private:
        /**
         * @brief Transform
        */
        Mathlib::Transform transform;
    public:
        Transform() = default;
        Transform(Mathlib::Transform _transform);
        ~Transform() = default;

        void Initialize();
        void Start();
        void Update();

        const Mathlib::Transform& GetLocalTransform() const;
        void SetLocalTransform(const Mathlib::Transform& _transform);
    };
}

#endif