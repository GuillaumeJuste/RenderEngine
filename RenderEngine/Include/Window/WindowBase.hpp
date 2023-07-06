#pragma once

#ifndef RENDERENGINE_WINDOWBASE
#define RENDERENGINE_WINDOWBASE

#include "Utils/Event/Event.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec2.hpp"

namespace RenderEngine::Window
{
    /**
     * @brief WindowBase class implementation
    */
    class WindowBase
    {
    protected:
        /**
         * @brief window width
        */
        unsigned int width;

        /**
         * @brief window height
        */
        unsigned int height;

        /**
         * @brief window name
        */
        const char* name;
    public:
        /**
         * @brief Event called when window is resized;
        */
        Utils::Event<> FramebufferResizeEvent;

        WindowBase() = default;
        ~WindowBase() = default;

        /**
         * @brief window update function
        */
        virtual void Update() = 0;

        /**
         * @brief cleaning function
        */
        virtual void Cleanup() = 0;

        /**
         * @brief Get window dimensions
         * @return window dimensions as VkExtent2D
        */
        virtual Mathlib::Vec2 GetWindowExtent() = 0;

        virtual bool WindowShouldClose() = 0;

        virtual void GetFrameBufferSize(int* _width, int* _height) = 0;

        virtual void* GetHandle() const = 0;
    };
}

#endif