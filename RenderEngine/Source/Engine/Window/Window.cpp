#include "Engine/Window/Window.hpp"
#include "Engine/Application/GraphicsApplication.hpp"

using namespace RenderEngine;

Window::Window(unsigned int _width, unsigned int _height, const char* _name) :
    width{ _width }, height{ _height }, name{ _name }
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfw_window = glfwCreateWindow(_width, _height, _name, nullptr, nullptr);
    glfwSetWindowUserPointer(glfw_window, this);
    glfwSetFramebufferSizeCallback(glfw_window, FramebufferResizeCallback);
}

void Window::Update()
{

}

void Window::Cleanup()
{
    glfwDestroyWindow(glfw_window);

    glfwTerminate();
}

GLFWwindow* Window::GetGLFWWindow()
{
    return glfw_window;
}

void Window::FramebufferResizeCallback(GLFWwindow* _window, int _width, int _height)
{
    Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(_window));
    window->frameBufferResized = true;
    window->height = _height;
    window->width = _width;
}

const bool& Window::GetFrameBufferResized() const
{
    return frameBufferResized;
}

void Window::SetFrameBufferResized(bool _val)
{
    frameBufferResized = _val;
}

VkExtent2D Window::GetWindowExtent()
{
    return VkExtent2D {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
    };
}
