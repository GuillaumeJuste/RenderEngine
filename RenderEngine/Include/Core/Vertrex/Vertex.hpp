#pragma once

#ifndef RENDERENGINE_VERTEX
#define RENDERENGINE_VERTEX

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"

namespace RenderEngine::Core
{
    /**
     * @brief Vertex data storing struct
    */
    struct Vertex 
    {
        /**
         * @brief vertex position
        */
        Mathlib::Vec3 position;

        /**
         * @brief vertex color
        */
        Mathlib::Vec3 color;
    };
}

#endif