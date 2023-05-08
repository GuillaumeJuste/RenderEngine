#pragma once

#ifndef ROTATORCOMPONENT
#define ROTATORCOMPONENT

#include "Core/Components/Component.hpp"


enum class ROTATION_AXIS
{
    X,
    Y,
    Z
};

class RotatorComponent : public RenderEngine::Core::Component
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