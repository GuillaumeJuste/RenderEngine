#pragma once

#ifndef RENDERENGINE_CAMERA
#define RENDERENGINE_CAMERA

#include "Collections/Mathlib.hpp"
#include "SceneGraph/Object/GameObject/GameObject.hpp"

#ifdef near
#undef near
#endif

#ifdef far
#undef far
#endif

namespace RenderEngine::SceneGraph
{
    /**
     * @brief Camera class implementation
    */
    class Camera : public GameObject
    {
    public:
        /// camera field of view
        float fov;

        /// camera near clipping plane
        float near;

        /// camera far clipping plane
        float far;

        /// constructor
        Camera();

        /// default destructor
        ~Camera() = default;

        /**
         * @brief Get the camera projection matrix
         * @param _windowAspectRatio window aspect ratio
        */
        Mathlib::Mat4 GetProjectionMatrix(float _windowAspectRatio) const;

        /**
         * @brief Get the camera Inverse view matrix 
        */
        Mathlib::Mat4 GetInvViewMatrix() const;
    };
}

#endif