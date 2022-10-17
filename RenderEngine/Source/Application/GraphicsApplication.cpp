#include "Application/GraphicsApplication.hpp"
#include "Core/Object/GameObject/GameObject.hpp"
#include "Core/Components/MeshRenderer/MeshRenderer.hpp"
#include "Application/CustomComponents/RotatorComponent.hpp"
#include <iostream>


using namespace RenderEngine;
using namespace RenderEngine::Engine::Vulkan;
using namespace RenderEngine::Core;
using namespace RenderEngine::Window;

void GraphicsApplication::Run()
{
    InitWindow();
    ressourceManager = RessourceManager::GetInstance();
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

Scene* GraphicsApplication::SetupTestScene()
{
    Scene* scene = sceneManager.AddScene();
    scene->name = "test_scene_1";

    Mesh* mesh = RessourceManager::GetInstance()->LoadMesh("Resources/Models/viking_room.obj", "viking_room");

    Texture* texture = RessourceManager::GetInstance()->LoadTexture("Resources/Textures/viking_room.png");
    Texture* texture2 = RessourceManager::GetInstance()->LoadTexture("Resources/Textures/texture.jpg");

    Mathlib::Transform transform;
    transform.position = Mathlib::Vec3(-2.f, 3.0f, 0.f);
    transform.scale = Mathlib::Vec3(1.f, 1.f, 1.f);
    transform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(45.f, 0.f, 0.f));

    GameObjectCreateInfo createinfo;
    createinfo.transform = transform;
    createinfo.parent = nullptr;
    createinfo.name = "first_object";

    GameObject* obj = scene->AddGameObject(createinfo);

    MeshRenderer* meshRenderer = obj->GetComponent<MeshRenderer>();
    meshRenderer->SetMesh(mesh);
    meshRenderer->SetTexture(texture);

    Mathlib::Transform transform2;
    transform2.position = Mathlib::Vec3(2.f, 3.0f, 0.f);
    transform2.scale = Mathlib::Vec3(0.5f, 0.5f, 0.5f);

    GameObjectCreateInfo createinfo2;
    createinfo2.transform = transform2;
    createinfo2.parent = nullptr;
    createinfo2.name = "second_object";

    GameObject* obj2 = scene->AddGameObject(createinfo2);
    MeshRenderer* meshRenderer2 = obj2->GetComponent<MeshRenderer>();
    meshRenderer2->SetTexture(texture2);

    RotatorComponent* rotator2 = obj2->AddComponent <RotatorComponent>();
    rotator2->rotationAxis = ROTATION_AXIS::Z;


    Mathlib::Transform transform3;
    transform3.position = Mathlib::Vec3(0.f, -2.0f, 0.f);
    transform3.scale = Mathlib::Vec3(1.5f, 1.5f, 1.5f);

    GameObjectCreateInfo createinfo3;
    createinfo3.transform = transform3;
    createinfo3.parent = nullptr;
    createinfo3.name = "third_object";

    GameObject* obj3 = scene->AddGameObject(createinfo3);

    Mathlib::Transform transform4;
    transform4.position = Mathlib::Vec3(1.5f, 0.0f, 0.f);
    transform4.scale = Mathlib::Vec3(0.5f, 0.5f, 0.5f);

    GameObjectCreateInfo createinfo4;
    createinfo4.transform = transform4;
    createinfo4.parent = obj2;
    createinfo4.name = "fourth_object";

    GameObject* obj4 = scene->AddGameObject(createinfo4);


    return scene;
}

void GraphicsApplication::MainLoop()
{
    Scene* scene = SetupTestScene();

    scene->Initialize();
    scene->Start();

    while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
        glfwPollEvents();
        renderContext->DrawScene(scene);
        scene->Update();
    }
    deviceContext->WaitDeviceIdle();
}

void GraphicsApplication::Cleanup()
{
    vulkanContext.Cleanup();

    window->Cleanup();
    delete window;
}