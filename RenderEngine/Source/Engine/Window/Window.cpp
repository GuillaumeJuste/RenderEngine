#include "Engine/Window/Window.hpp"

using namespace RenderEngine;

Window::Window(unsigned int _width, unsigned int _height, const char* _name) :
    width{ _width }, height{ _height }, name{ _name }
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glfw_window = glfwCreateWindow(_width, _height, _name, nullptr, nullptr);
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