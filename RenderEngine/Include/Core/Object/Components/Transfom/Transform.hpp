#pragma once

#ifndef RENDERENGINE_TRANSFORM
#define RENDERENGINE_TRANSFORM

#include "Core/Object/Components/Component.hpp"
#include "Transform/Transform.hpp"

namespace RenderEngine::Core
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