#pragma once

#ifndef CAMERACONTROLLER
#define CAMERACONTROLLER

#include "SceneGraph/Components/Component.hpp"
#include "Window/IWindow.hpp"

class CameraController : public RenderEngine::SceneGraph::Component
{
private:
    double oldMouseX = 0.0;
    double oldMouseY =0.0;

    float dx = 0.0f;
    float dy = 0.0f;

public:
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


#endif