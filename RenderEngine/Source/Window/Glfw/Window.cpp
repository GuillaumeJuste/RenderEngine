#include "Window/Glfw/Window.hpp"

#include <iostream>

using namespace RenderEngine::Window::GLFW;

Window::Window(unsigned int _width, unsigned int _height, const char* _name) :
    width{ _width }, height{ _height }, name{ _name }
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfwWindow = glfwCreateWindow(_width, _height, _name, nullptr, nullptr);
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetFramebufferSizeCallback(glfwWindow, FramebufferResizeCallback);
}

void Window::Update()
{

}

void Window::Cleanup()
{
    glfwDestroyWindow(glfwWindow);

    glfwTerminate();
    std::cout << "[Cleaned] Window" << std::endl;
}

GLFWwindow* Window::GetGLFWWindow()
{
    return glfwWindow;
}

void Window::FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height)
{
    Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(_window));
    window->height = _height;
    window->width = _width;
    window->FramebufferResizeEvent();
}

VkExtent2D Window::GetWindowExtent()
{
    return VkExtent2D {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
    };
}
