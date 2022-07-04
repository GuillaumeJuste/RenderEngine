#include "Engine/GraphicsApplication.hpp"
#include "Engine/DeviceContext.hpp"
#include "Engine/Window.hpp"
#include <iostream>

using namespace RenderEngine;

void GraphicsApplication::Run()
{
    InitWindow();
    InitVulkan();
    MainLoop();
    Cleanup();
}

void GraphicsApplication::InitWindow()
{
    std::cout << "[Initialize] Window" << std::endl;
    window = new Window(1024, 720, "Render Engine");
}

void GraphicsApplication::InitVulkan()
{
    std::cout << "[Initialize] Vulkan" << std::endl;
    deviceContext = new DeviceContext();
}

void GraphicsApplication::MainLoop()
{
    while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
        glfwPollEvents();
    }
}

void GraphicsApplication::Cleanup()
{
    deviceContext->Cleanup();
    delete deviceContext;
    std::cout << "[Deleted] Vulkan context" << std::endl;

    delete window;
    std::cout << "[Deleted] Window" << std::endl;
}