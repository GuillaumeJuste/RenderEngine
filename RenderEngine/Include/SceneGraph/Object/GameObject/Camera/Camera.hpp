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
    class Camera : public GameObject
    {
    public:

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