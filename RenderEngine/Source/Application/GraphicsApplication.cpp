#include "Application/GraphicsApplication.hpp"
#include "Engine/Device/DeviceContext.hpp"
#include "Engine/Window/Window.hpp"
#include <iostream>

using namespace RenderEngine;
using namespace RenderEngine::Vulkan;

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
    deviceContext = new DeviceContext(window);
}

void GraphicsApplication::MainLoop()
{
    while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
        glfwPollEvents();
        deviceContext->GetDevice()->DrawFrame();
    }
    vkDeviceWaitIdle(deviceContext->GetDevice()->GetLogicalDevice());
}

void GraphicsApplication::Cleanup()
{
    deviceContext->Cleanup();
    delete deviceContext;

    window->Cleanup();
    delete window;
}