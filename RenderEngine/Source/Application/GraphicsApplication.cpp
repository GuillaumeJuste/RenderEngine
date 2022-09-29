#include "Application/GraphicsApplication.hpp"
#include "Core/Object/GameObject/GameObject.hpp"
#include "Core/Object/Components/MeshRenderer/MeshRenderer.hpp"
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

    GraphicsPipelineCreateInfo gPCreateInfo{};
    gPCreateInfo.vertexShaderFilePath = "Resources/Shaders/VertexShader.spv";
    gPCreateInfo.fragmentShaderFilePath = "Resources/Shaders/FragmentShader.spv";
    gPCreateInfo.drawMode = PolygonDrawMode::FILL;
    gPCreateInfo.frontFace = FrontFace::CLOCKWISE;

    SwapChainCommandBufferCreateInfo CBCreateInfo{};
    CBCreateInfo.customViewport = false;
    CBCreateInfo.customScissor = false;

    IRenderContextCreateInfo renderContextCreateInfo {};
    renderContextCreateInfo.graphicsPipelineCreateInfo = &gPCreateInfo;
    renderContextCreateInfo.swapChainCommandBufferCreateInfo = &CBCreateInfo;
    renderContext = deviceContext->CreateRenderContext(renderContextCreateInfo);
}

std::string GraphicsApplication::UserSelectPhysicalDevice(std::vector<std::string> _physicalDevicesNames)
{
    size_t size = _physicalDevicesNames.size();
    

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
    Scene* scene = SceneManager.AddScene();
    scene->name = "test_scene_1";

    const std::vector<Vertex> vertices = {
    {{-0.75f, -0.75f}, {1.0f, 0.0f, 0.0f}},
    {{-0.25f, -0.75f}, {0.0f, 1.0f, 0.0f}},
    {{-0.25f, -0.25f}, {0.0f, 0.0f, 1.0f}},
    {{-0.75f, -0.25f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<Vertex> vertices2 = {
    {{0.25f, 0.25f}, {1.0f, 0.0f, 0.0f}},
    {{0.75f, 0.25f}, {0.0f, 1.0f, 0.0f}},
    {{0.75f, 0.75f}, {0.0f, 0.0f, 1.0f}},
    {{0.25f, 0.75f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };
    
    Mesh mesh;
    Mesh::InitializeMesh(vertices, indices, &mesh);

    Mesh mesh2;
    Mesh::InitializeMesh(vertices, indices, &mesh2);

    Mathlib::Transform transform;

    GameObjectCreateInfo createinfo;
    createinfo.transform = transform;
    createinfo.parent = scene->GetSceneRoot();
    createinfo.name = "first_object";

    GameObject* obj = scene->AddGameObject(createinfo);
    MeshRenderer* meshRenderer = obj->AddComponent<MeshRenderer>();
    meshRenderer->SetMesh(mesh);

    GameObjectCreateInfo createinfo2;
    createinfo2.transform = transform;
    createinfo2.parent = scene->GetSceneRoot();
    createinfo2.name = "second_object";

    GameObject* obj2 = scene->AddGameObject(createinfo2);
    MeshRenderer* meshRenderer2 = obj2->AddComponent<MeshRenderer>();
    meshRenderer2->SetMesh(mesh2);

    scene->Initialize();
    scene->Start();

    while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
        glfwPollEvents();
        //renderContext->DrawFrame();
        renderContext->DrawScene(scene);
        //scene.Update();
    }
    deviceContext->WaitDeviceIdle();
}

void GraphicsApplication::Cleanup()
{
    vulkanContext.Cleanup();

    window->Cleanup();
    delete window;
}