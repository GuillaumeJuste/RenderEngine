#pragma once

#ifndef ROTATORCOMPONENT
#define ROTATORCOMPONENT

#include "SceneGraph/Components/Component.hpp"


enum class ROTATION_AXIS
{
    X,
    Y,
    Z
};

class RotatorComponent : public RenderEngine::SceneGraph::Component
{
private:
    float currentRotation = 0;
public:
    ROTATION_AXIS rotationAxis = ROTATION_AXIS::X;

    RotatorComponent() = default;
    ~RotatorComponent() = default;

    void Initialize();
    void Start();
    void Update();
};


#endif