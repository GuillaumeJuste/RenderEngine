#include "Application/GraphicsApplication.hpp"
#include "Core/Object/GameObject/GameObject.hpp"

#include <iostream>


using namespace RenderEngine;
using namespace RenderEngine::Engine::Vulkan;
using namespace RenderEngine::Core;
using namespace RenderEngine::Window;

void GraphicsApplication::Run()
{
    InitWindow();
    InitEngine();
    MainLoop();
    Cleanup();
}

void GraphicsApplication::InitWindow()
{
    std::cout << "[Initialize] Window" << std::endl;
    window = new GLFW::Window(1024, 720, "Render Engine");
}

void GraphicsApplication::InitEngine()
{
    std::cout << "[Initialize] Vulkan" << std::endl;
    IEngineInstanceCreateInfo instanceCreateInfo {};
    instanceCreateInfo.applicationName = "RenderEngine";
    instanceCreateInfo.applicationVersion = Mathlib::Vec3(1.f, 0.f, 0.f);
    instanceCreateInfo.engineName = "VulkanRenderer";
    instanceCreateInfo.engineVersion = Mathlib::Vec3(1.f , 0.f, 0.f);
    vulkanContext.InitializeInstance(instanceCreateInfo);

    IDeviceContextCreateInfo deviceCreateInfo {};
    deviceCreateInfo.window = window;
    deviceContext = vulkanContext.CreateDeviceContext(deviceCreateInfo);
    std::vector<std::string> deviceNames = deviceContext->QueryAvailblePhysicalDevices();
    deviceContext->InitializeDeviceContext(UserSelectPhysicalDevice(deviceNames));

    IRenderContextCreateInfo renderContextCreateInfo {};
    renderContext = deviceContext->CreateRenderContext(renderContextCreateInfo);
}

std::string GraphicsApplication::UserSelectPhysicalDevice(std::vector<std::string> _physicalDevicesNames)
{
    int size = _physicalDevicesNames.size();
    

    if(size == 0)
        throw std::runtime_error("No GPU listed");

    else if(size == 1)
        return _physicalDevicesNames[0];
    
    std::cout << "Pick GPU for rendering :" << std::endl;

    for (int i = 0; i < size; i++)
    {
        std::cout << "[" << i << "] : " << _physicalDevicesNames[i] << std::endl;
    }
    int gpuIndex = -1;
    while (gpuIndex < 0 || gpuIndex >= size)
    {
        std::cin >> gpuIndex;
    }

    return _physicalDevicesNames[gpuIndex];
}

void GraphicsApplication::MainLoop()
{
    SceneCreateInfo sceneInfo;
    sceneInfo.name = "test_scene_1";

    Scene::InitializeScene(sceneInfo, &scene);

    const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0
    };
    
    Mesh mesh;
    Mesh::InitializeMesh(vertices, indices, &mesh);

    Mathlib::Transform transform;

    GameObjectCreateInfo createinfo;
    createinfo.mesh = &mesh;
    createinfo.transform = transform;
    createinfo.parent = scene.GetSceneRoot();
    createinfo.name = "first_object";

    GameObject* obj = scene.AddGameObject(createinfo);
    scene.RemoveGameObject(obj);

    if (obj == obj)

    while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
        glfwPollEvents();
        renderContext->DrawFrame();
    }
    deviceContext->WaitDeviceIdle();
}

void GraphicsApplication::Cleanup()
{
    vulkanContext.Cleanup();

    window->Cleanup();
    delete window;
}