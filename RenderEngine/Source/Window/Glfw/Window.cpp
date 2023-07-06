#include "Window/Glfw/Window.hpp"

#include <iostream>

using namespace RenderEngine::Window::GLFW;

Window::Window(unsigned int _width, unsigned int _height, const char* _name)
{
    width = _width;
    height = _height;
    name = _name;
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfwWindow = glfwCreateWindow(_width, _height, _name, nullptr, nullptr);
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetFramebufferSizeCallback(glfwWindow, FramebufferResizeCallback);

    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::Update()
{
    glfwPollEvents();
}

void Window::Cleanup()
{
    glfwDestroyWindow(glfwWindow);

    glfwTerminate();
}

void Window::FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height)
{
    Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(_window));
    window->height = _height;
    window->width = _width;
    window->FramebufferResizeEvent();
}

Mathlib::Vec2 Window::GetWindowExtent()
{
    return Mathlib::Vec2 {
            static_cast<float>(width),
            static_cast<float>(height)
    };
}

bool Window::WindowShouldClose()
{
    return glfwWindowShouldClose(glfwWindow);
}

void Window::GetFrameBufferSize(int* _width, int* _height)
{
    glfwGetFramebufferSize(glfwWindow, _width, _height);
    glfwWaitEvents();
}

void* Window::GetHandle() const
{
    return glfwWindow;
}

bool Window::GetKeyPressed(int _keycode, int _keyStatus)
{
    return glfwGetKey(glfwWindow, _keycode) == _keyStatus;
}

void Window::GetCursorPos(double* _mouseX, double* _mouseY)
{
    glfwGetCursorPos(glfwWindow, _mouseX, _mouseY);
}