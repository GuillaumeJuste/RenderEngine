#pragma once

#ifndef RENDERENGINE_IWINDOW
#define RENDERENGINE_IWINDOW

#include "Utils/Event/Event.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec2.hpp"
#include "Utils/Input/Input.hpp"

namespace RenderEngine::Window
{
    /**
     * @brief WindowBase class implementation
    */
    class IWindow
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

        bool mouseLocked = false;
    public:
        /**
         * @brief Event called when window is resized;
        */
        Utils::Event<> FramebufferResizeEvent;

        IWindow() = default;
        ~IWindow() = default;

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

        virtual bool GetKeyPressed(RenderEngine::Utils::Input _key, RenderEngine::Utils::InputStatus _keyStatus) = 0;
        virtual void GetCursorPos(double* _mouseX, double* _mouseY) = 0;

        virtual void LockMouseToWindow(bool _enable) = 0;
    };
}

#endif