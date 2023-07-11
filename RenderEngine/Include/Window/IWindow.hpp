#pragma once

#ifndef RENDERENGINE_IWINDOW
#define RENDERENGINE_IWINDOW

#include "Utils/Event/Event.hpp"
#include "Mathlib/Mathlib/Include/Space/Vec2.hpp"
#include "Utils/Input/Input.hpp"

namespace RenderEngine::Window
{
    /**
     * @brief IWindow class implementation
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
         * @return window dimensions as Mathlib::Vec2
        */
        virtual Mathlib::Vec2 GetWindowExtent() = 0;

        /**
         * @brief Check the close flag of the window
         * @return true if the window should be closed
        */
        virtual bool WindowShouldClose() = 0;

        /**
         * @brief Get the framebuffer size
         * @param _width output width
         * @param _height output height
        */
        virtual void GetFrameBufferSize(int* _width, int* _height) = 0;

        /**
         * @brief Get the window handle as a void*
         * @return window handle
        */
        virtual void* GetHandle() const = 0;

        /**
         * @brief Check the input status of a key if the window is in use
         * @param _key Key to check
         * @param _keyStatus state of the key to check
         * @return true if the key status match _keyStatus
        */
        virtual bool CheckKeyStatus(RenderEngine::Utils::Input _key, RenderEngine::Utils::InputStatus _keyStatus) = 0;

        /**
         * @brief Get  the cursor position inside the window
         * @param _mouseX output X coordinate
         * @param _mouseY outpur Y coordinate
        */
        virtual void GetCursorPos(double* _mouseX, double* _mouseY) = 0;

        /**
         * @brief Lock the cursor inside the window
         * @param _enable enabling status
        */
        virtual void LockMouseToWindow(bool _enable) = 0;
    };
}

#endif