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
         * @brief object Transform
        */
        Mathlib::Transform transform;
    public:
        /// default constructor
        Transform() = default;

        /**
         * @brief constructor from Mathlib::Transform
         * @param _transform transform to set the object transform to
        */
        Transform(Mathlib::Transform _transform);
        
        /// default destructor
        ~Transform() = default;
        
        
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
        
        /**
         * @brief Get the GameObject local transform
        */
        const Mathlib::Transform& GetLocalTransform() const;
        
        /**
         * @brief Set the GameObject transform
         * @param _transform GameObject new transform
        */
        void SetLocalTransform(const Mathlib::Transform& _transform);
    };
}

#endif