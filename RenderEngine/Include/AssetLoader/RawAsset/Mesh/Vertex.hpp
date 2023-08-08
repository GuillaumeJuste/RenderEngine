#pragma once

#ifndef RENDERENGINE_VERTEX
#define RENDERENGINE_VERTEX

#include "Mathlib/Mathlib/Include/Space/Vec3.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec4.hpp"

namespace Loader
{
    /**
     * @brief Vertex data storing struct
    */
    struct Vertex 
    {
        
        ///vertex position
        Mathlib::Vec3 position;

        /// vertex normal
        Mathlib::Vec3 normal;
        
        /// vertex texture coordinate
        Mathlib::Vec3 textCoord;

        /// vertex tangent
        Mathlib::Vec3 tangent;
    };
}

#endif