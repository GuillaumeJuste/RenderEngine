#pragma once

#ifndef RENDERENGINE_CAMERA
#define RENDERENGINE_CAMERA

#include "Collections/Space.hpp"

#ifdef near
#undef near
#endif

#ifdef far
#undef far
#endif

namespace RenderEngine::Core
{
    class Camera
    {
    public:

        Mathlib::Vec3 position;
        Mathlib::Vec3 lookAt;
        Mathlib::Vec3 up;

        float fov;
        float near;
        float far;

        Camera();
        ~Camera() = default;

        Mathlib::Mat4 GetProjectionMatrix(float _windowAspectRatio) const;
        Mathlib::Mat4 GetViewMatrix() const;
    };
}

#endif