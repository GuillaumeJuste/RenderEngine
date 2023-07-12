#pragma once

#ifndef CAMERACONTROLLER
#define CAMERACONTROLLER

#include "SceneGraph/Components/Component.hpp"
#include "Window/IWindow.hpp"

namespace RenderEngine::Component
{
    class CameraController : public RenderEngine::SceneGraph::Component
    {
    private:
        double oldMouseX = 0.0;
        double oldMouseY = 0.0;

        float dx = 0.0f;
        float dy = 0.0f;

        bool noUpdate = false;
        bool enableMouse = false;

    public:
        RenderEngine::Utils::Input forward = RenderEngine::Utils::Input::KEY_UP;
        RenderEngine::Utils::Input back = RenderEngine::Utils::Input::KEY_DOWN;
        RenderEngine::Utils::Input left = RenderEngine::Utils::Input::KEY_LEFT;
        RenderEngine::Utils::Input right = RenderEngine::Utils::Input::KEY_RIGHT;
        RenderEngine::Utils::Input up = RenderEngine::Utils::Input::KEY_SPACE;
        RenderEngine::Utils::Input down = RenderEngine::Utils::Input::KEY_LEFT_CONTROL;
        RenderEngine::Utils::Input LockCursorToWindow = RenderEngine::Utils::Input::KEY_F1;

        RenderEngine::Window::IWindow* window;
        float movementSpeed = 20.f;
        float rotationSpeed = 10.f;

        CameraController() = default;
        ~CameraController() = default;

        void Initialize();
        void Start();
        void Update(double _deltaTime);
        void FixedUpdate(double _deltaTime);
    };
}

#endif