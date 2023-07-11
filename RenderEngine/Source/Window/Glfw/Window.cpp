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

bool Window::CheckKeyStatus(RenderEngine::Utils::Input _key, RenderEngine::Utils::InputStatus _keyStatus)
{
    return glfwGetKey(glfwWindow, GetKeyValue(_key)) == GetKeyStatusValue(_keyStatus);
}

void Window::GetCursorPos(double* _mouseX, double* _mouseY)
{
    glfwGetCursorPos(glfwWindow, _mouseX, _mouseY);
}

void Window::LockMouseToWindow(bool _enable)
{
    if (_enable != mouseLocked)
    {
        mouseLocked = _enable;
        if(mouseLocked)
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    }
}

static const int glewKeyIndexMap[] = 
{
    GLFW_KEY_UNKNOWN,
    GLFW_KEY_SPACE,
    GLFW_KEY_APOSTROPHE,
    GLFW_KEY_COMMA,
    GLFW_KEY_MINUS,
    GLFW_KEY_PERIOD,
    GLFW_KEY_SLASH,
    GLFW_KEY_0,
    GLFW_KEY_1,
    GLFW_KEY_2,
    GLFW_KEY_3,
    GLFW_KEY_4,
    GLFW_KEY_5,
    GLFW_KEY_6,
    GLFW_KEY_7,
    GLFW_KEY_8,
    GLFW_KEY_9,
    GLFW_KEY_SEMICOLON,
    GLFW_KEY_EQUAL,
    GLFW_KEY_A,
    GLFW_KEY_B,
    GLFW_KEY_C,
    GLFW_KEY_D,
    GLFW_KEY_E,
    GLFW_KEY_F,
    GLFW_KEY_G,
    GLFW_KEY_H,
    GLFW_KEY_I,
    GLFW_KEY_J,
    GLFW_KEY_K,
    GLFW_KEY_L,
    GLFW_KEY_M,
    GLFW_KEY_N,
    GLFW_KEY_O,
    GLFW_KEY_P,
    GLFW_KEY_Q,
    GLFW_KEY_R,
    GLFW_KEY_S,
    GLFW_KEY_T,
    GLFW_KEY_U,
    GLFW_KEY_V,
    GLFW_KEY_W,
    GLFW_KEY_X,
    GLFW_KEY_Y,
    GLFW_KEY_Z,
    GLFW_KEY_LEFT_BRACKET,
    GLFW_KEY_BACKSLASH,
    GLFW_KEY_RIGHT_BRACKET,
    GLFW_KEY_GRAVE_ACCENT,
    GLFW_KEY_WORLD_1,
    GLFW_KEY_WORLD_2,
    GLFW_KEY_ESCAPE,
    GLFW_KEY_ENTER,
    GLFW_KEY_TAB,
    GLFW_KEY_BACKSPACE,
    GLFW_KEY_INSERT,
    GLFW_KEY_DELETE,
    GLFW_KEY_RIGHT,
    GLFW_KEY_LEFT,
    GLFW_KEY_DOWN,
    GLFW_KEY_UP,
    GLFW_KEY_PAGE_UP,
    GLFW_KEY_PAGE_DOWN,
    GLFW_KEY_HOME,
    GLFW_KEY_END,
    GLFW_KEY_CAPS_LOCK,
    GLFW_KEY_SCROLL_LOCK,
    GLFW_KEY_NUM_LOCK,
    GLFW_KEY_PRINT_SCREEN,
    GLFW_KEY_PAUSE,
    GLFW_KEY_F1,
    GLFW_KEY_F2,
    GLFW_KEY_F3,
    GLFW_KEY_F4,
    GLFW_KEY_F5,
    GLFW_KEY_F6,
    GLFW_KEY_F7,
    GLFW_KEY_F8,
    GLFW_KEY_F9,
    GLFW_KEY_F10,
    GLFW_KEY_F11,
    GLFW_KEY_F12,
    GLFW_KEY_F13,
    GLFW_KEY_F14,
    GLFW_KEY_F15,
    GLFW_KEY_F16,
    GLFW_KEY_F17,
    GLFW_KEY_F18,
    GLFW_KEY_F19,
    GLFW_KEY_F20,
    GLFW_KEY_F21,
    GLFW_KEY_F22,
    GLFW_KEY_F23,
    GLFW_KEY_F24,
    GLFW_KEY_F25,
    GLFW_KEY_KP_0,
    GLFW_KEY_KP_1,
    GLFW_KEY_KP_2,
    GLFW_KEY_KP_3,
    GLFW_KEY_KP_4,
    GLFW_KEY_KP_5,
    GLFW_KEY_KP_6,
    GLFW_KEY_KP_7,
    GLFW_KEY_KP_8,
    GLFW_KEY_KP_9,
    GLFW_KEY_KP_DECIMAL,
    GLFW_KEY_KP_DIVIDE,
    GLFW_KEY_KP_MULTIPLY,
    GLFW_KEY_KP_SUBTRACT,
    GLFW_KEY_KP_ADD,
    GLFW_KEY_KP_ENTER,
    GLFW_KEY_KP_EQUAL,
    GLFW_KEY_LEFT_SHIFT,
    GLFW_KEY_LEFT_CONTROL,
    GLFW_KEY_LEFT_ALT,
    GLFW_KEY_LEFT_SUPER,
    GLFW_KEY_RIGHT_SHIFT,
    GLFW_KEY_RIGHT_CONTROL,
    GLFW_KEY_RIGHT_ALT,
    GLFW_KEY_RIGHT_SUPER,
    GLFW_KEY_MENU
};

int Window::GetKeyValue(RenderEngine::Utils::Input _key)
{
    const uint32_t index = static_cast<uint32_t>(_key);
    return glewKeyIndexMap[index];
}

static const int glewKeyStatusIndexMap[] =
{
    GLFW_RELEASE,
    GLFW_PRESS,
    GLFW_REPEAT
};

int Window::GetKeyStatusValue(RenderEngine::Utils::InputStatus _keyStatus)
{
    const uint32_t index = static_cast<uint32_t>(_keyStatus);
    return glewKeyStatusIndexMap[index];
}