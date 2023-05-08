#pragma once

#ifndef RENDERENGINE_VERTEX
#define RENDERENGINE_VERTEX

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec4.hpp"

namespace RenderEngine::Assets
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

        Mathlib::Vec3 normal;
        
        Mathlib::Vec3 textCoord;
    };
}

#endif