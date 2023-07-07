#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "Rendering/Vulkan/Context/VulkanContext/VulkanContext.hpp"
#include "Window/Glfw/Window.hpp"
#include "SceneGraph/Scene/SceneManager.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "SceneGraph/Object/GameObject/GameObject.hpp"
#include "SceneGraph/Components/MeshRenderer/MeshRenderer.hpp"
#include "SceneGraph/Components/Light/PointLight.hpp"
#include "SceneGraph/Components/Light/DirectionalLight.hpp"
#include "SceneGraph/Components/Light/SpotLight.hpp"
#include "Component/RotatorComponent.hpp"
#include "Component/CameraController.hpp"
#include <iostream>

#include<Mathlib/Mathlib/Include/Misc/Math.hpp>

using namespace RenderEngine::Rendering;
using namespace RenderEngine;

/**
* @brief Window for engine rendering
*/
GLFW::Window* window;

/**
 * @brief vulkan context
*/
RenderEngine::Rendering::VulkanContext vulkanContext;

/**
 * @brief device used to render scene
*/
IDeviceContext* deviceContext;

/**
 * @brief render context to render scene
*/
IRenderContext* renderContext;

SceneManager* sceneManager;

ResourceManager* resourceManager;

using namespace RenderEngine;
using namespace RenderEngine::Rendering;
using namespace RenderEngine::Window;

void InitWindow()
{
    std::cout << "[Initialize] Window" << std::endl;
    window = new GLFW::Window(1024, 720, "Render Engine");
}

std::string UserSelectPhysicalDevice(std::vector<std::string> _physicalDevicesNames)
{
    size_t size = _physicalDevicesNames.size();


    if (size == 0)
        throw std::runtime_error("No GPU listed");

    else if (size == 1)
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

void InitEngine()
{
    std::cout << "[Initialize] Vulkan" << std::endl;
    IEngineInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.applicationName = "RenderEngine";
    instanceCreateInfo.applicationVersion = Mathlib::Vec3(1.f, 0.f, 0.f);
    instanceCreateInfo.engineName = "VulkanRenderer";
    instanceCreateInfo.engineVersion = Mathlib::Vec3(1.f, 0.f, 0.f);
    vulkanContext.InitializeInstance(instanceCreateInfo);

    IDeviceContextCreateInfo deviceCreateInfo{};
    deviceCreateInfo.window = window;
    deviceContext = vulkanContext.CreateDeviceContext(deviceCreateInfo);
    std::vector<std::string> deviceNames = deviceContext->QueryAvailblePhysicalDevices();
    deviceContext->InitializeDeviceContext(UserSelectPhysicalDevice(deviceNames));

    SwapChainCommandBufferCreateInfo CBCreateInfo{};
    CBCreateInfo.customViewport = false;
    CBCreateInfo.customScissor = false;

    IRenderContextCreateInfo renderContextCreateInfo{};
    renderContextCreateInfo.swapChainCommandBufferCreateInfo = &CBCreateInfo;
    renderContext = deviceContext->CreateRenderContext(renderContextCreateInfo);
}

Scene* SetupPBRScene()
{
    Scene* scene = sceneManager->AddScene();
    scene->name = "test_scene_simple_cube";

    Texture* skyboxTexture = resourceManager->LoadTexture("Resources/Engine/Textures/HDR/newport_loft.hdr", true, false);
    resourceManager->CreateSkyboxFromTexture(skyboxTexture, Mathlib::Vec2(1024.f, 1024.f), &scene->skybox);

    Camera* camera = scene->GetCamera();
    Mathlib::Transform cameraTransform;
    cameraTransform.position = Mathlib::Vec3(0.0f, 0.0f, -2.f);
    cameraTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));
    camera->SetLocalTransform(cameraTransform);
    camera->fov = 90.f;

    CameraController* cameraController = camera->AddComponent<CameraController>();
    cameraController->window = window;

    Mesh* sphere = resourceManager->LoadMesh("Resources/Sample/PBRScene/Models/Sphere.obj");
    Shader* fragShader = resourceManager->LoadShader("Resources/Engine/Shaders/PBRFragmentShader.frag.spv", FRAGMENT);
    Texture* wallTexture = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Wall/albedo.png");
    Texture* wallMetalnessMap = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Wall/metallic.png");
    Texture* wallRoughnessMap = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Wall/roughness.png");
    Texture* wallNormalMap = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Wall/normal.png");
    Texture* wallAoMap = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Wall/ao.png");

    /*Sphere 1*/
    Mathlib::Transform transform;
    transform.position = Mathlib::Vec3(-1.f, 0.f, 0.0f);
    transform.scale = Mathlib::Vec3(0.015f, 0.015f, 0.015f);

    GameObjectCreateInfo createinfo;
    createinfo.transform = transform;
    createinfo.parent = nullptr;
    createinfo.name = "sphere_1";

    GameObject* obj = scene->AddGameObject(createinfo);

    MeshRenderer* meshRenderer = obj->GetComponent<MeshRenderer>();
    meshRenderer->fragmentShader = fragShader;
    meshRenderer->mesh = sphere;
    meshRenderer->material.texture = wallTexture;
    meshRenderer->material.metalnessMap = wallMetalnessMap;
    meshRenderer->material.roughnessMap = wallRoughnessMap;
    meshRenderer->material.normalMap = wallNormalMap;
    meshRenderer->material.ambientOcclusionMap = wallAoMap;
    meshRenderer->material.shininess = 32.0f;
    meshRenderer->material.ambient = Mathlib::Vec4(0.1f, 0.1f, 0.1f, 1.f);
    meshRenderer->material.diffuse = Mathlib::Vec4(0.4f, 0.4f, 0.4f, 1.f);
    meshRenderer->material.specular = Mathlib::Vec4(0.8f, 0.8f, 0.8f, 1.f);

    RotatorComponent* rotator = obj->AddComponent<RotatorComponent>();
    rotator->rotationAxis = ROTATION_AXIS::Y;

    /*Sphere 2*/

    Mathlib::Transform transform2;
    transform2.position = Mathlib::Vec3(0.f, 0.f, 0.0f);
    transform2.scale = Mathlib::Vec3(0.015f, 0.015f, 0.015f);

    GameObjectCreateInfo createinfo2;
    createinfo2.transform = transform2;
    createinfo2.parent = nullptr;
    createinfo2.name = "sphere_2";

    GameObject* obj2 = scene->AddGameObject(createinfo2);

    Texture* ironTexture = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Rusted_iron/albedo.png");
    Texture* ironMetalnessMap = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Rusted_iron/metallic.png");
    Texture* ironRoughnessMap = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Rusted_iron/roughness.png");
    Texture* ironNormalMap = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Rusted_iron/normal.png");
    Texture* ironAoMap = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Rusted_iron/ao.png");

    MeshRenderer* meshRenderer2 = obj2->GetComponent<MeshRenderer>();
    meshRenderer2->fragmentShader = fragShader;
    meshRenderer2->mesh = sphere;
    meshRenderer2->material.texture = ironTexture;
    meshRenderer2->material.metalnessMap = ironMetalnessMap;
    meshRenderer2->material.roughnessMap = ironRoughnessMap;
    meshRenderer2->material.normalMap = ironNormalMap;
    meshRenderer2->material.ambientOcclusionMap = ironAoMap;
    meshRenderer2->material.shininess = 32.0f;
    meshRenderer2->material.ambient = Mathlib::Vec3(0.1f, 0.1f, 0.1f);
    meshRenderer2->material.diffuse = Mathlib::Vec3(0.5f, 0.5f, 0.5f);
    meshRenderer2->material.specular = Mathlib::Vec3(1.0f, 1.0f, 1.0f);

    RotatorComponent* rotator2 = obj2->AddComponent<RotatorComponent>();
    rotator2->rotationAxis = ROTATION_AXIS::Y;

    /*Sphere 3*/

    Mathlib::Transform transform3;
    transform3.position = Mathlib::Vec3(1.f, 0.f, 0.0f);
    transform3.scale = Mathlib::Vec3(0.015f, 0.015f, 0.015f);

    GameObjectCreateInfo createinfo3;
    createinfo3.transform = transform3;
    createinfo3.parent = nullptr;
    createinfo3.name = "sphere_3";

    GameObject* obj3 = scene->AddGameObject(createinfo3);

    Texture* goldTexture = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Gold/albedo.png");
    Texture* goldMetalnessMap = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Gold/metallic.png");
    Texture* goldRoughnessMap = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Gold/roughness.png");
    Texture* goldNormalMap = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Gold/normal.png");
    Texture* goldAoMap = resourceManager->LoadTexture("Resources/Sample/PBRScene/Textures/Gold/ao.png");

    MeshRenderer* meshRenderer3 = obj3->GetComponent<MeshRenderer>();
    meshRenderer3->mesh = sphere;
    meshRenderer3->material.texture = goldTexture;
    meshRenderer3->material.metalnessMap = goldMetalnessMap;
    meshRenderer3->material.roughnessMap = goldRoughnessMap;
    meshRenderer3->material.normalMap = goldNormalMap;
    meshRenderer3->material.ambientOcclusionMap = goldAoMap;
    meshRenderer3->fragmentShader = fragShader;
    meshRenderer3->material.shininess = 32.0f;
    meshRenderer3->material.ambient = Mathlib::Vec3(0.1f, 0.1f, 0.1f);
    meshRenderer3->material.diffuse = Mathlib::Vec3(0.5f, 0.5f, 0.5f);
    meshRenderer3->material.specular = Mathlib::Vec3(1.0f, 1.0f, 1.0f);

    RotatorComponent* rotator3 = obj3->AddComponent<RotatorComponent>();
    rotator3->rotationAxis = ROTATION_AXIS::Y;

    /*light 1*/
    Mathlib::Transform transform4;
    transform4.position = Mathlib::Vec3(0.f, 0.0f, -1.f);
    transform4.scale = Mathlib::Vec3(0.1f, 0.1f, 0.1f);

    GameObjectCreateInfo createinfo4;
    createinfo4.transform = transform4;
    createinfo4.parent = nullptr;
    createinfo4.name = "light_4";

    GameObject* light4 = scene->AddGameObject(createinfo4);

    PointLight* lightComponent4 = light4->AddComponent<PointLight>();
    lightComponent4->color = Mathlib::Vec3(1.0f, 1.0f, 1.f);
    lightComponent4->range = 30.f;
    lightComponent4->intensity = 1.f;

    MeshRenderer* meshRenderer4 = light4->GetComponent<MeshRenderer>();
    meshRenderer4->enable = true;
    meshRenderer4->fragmentShader = resourceManager->LoadShader("Resources/Engine/Shaders/TextureFragmentShader.frag.spv", FRAGMENT);
    meshRenderer4->frontFace = FrontFace::COUNTER_CLOCKWISE;

    return scene;
}

void MainLoop()
{
    Scene* scene = SetupPBRScene();

    scene->Initialize();
    scene->Start();

    while (!window->WindowShouldClose()) 
    {
        window->Update();
        renderContext->DrawScene(scene);
        scene->Update();
    }
    deviceContext->WaitDeviceIdle();
}

void Cleanup()
{
    delete sceneManager;
    resourceManager->Clean();
    delete resourceManager;
    vulkanContext.Cleanup();
    window->Cleanup();
    delete window;
}

void Run()
{
    InitWindow();
    InitEngine();
    resourceManager = new ResourceManager(renderContext);
    sceneManager = new SceneManager(resourceManager);
    MainLoop();
    Cleanup();
}


int main() 
{
    try {
        Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}