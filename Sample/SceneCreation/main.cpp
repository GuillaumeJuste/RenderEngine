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

Scene* SetupIlluminationScene()
{
    Scene* scene = sceneManager->AddScene();
    scene->name = "test_scene_illumination";

    Camera* camera = scene->GetCamera();
    Mathlib::Transform cameraTransform;
    cameraTransform.position = Mathlib::Vec3(0.0f, 0.0f, -6.0f);
    cameraTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));
    camera->SetLocalTransform(cameraTransform);
    camera->fov = 90.f;

    Texture* texture = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/container.png");
    Texture* specularMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/container_specular.png");

    Mesh* sphere = resourceManager->LoadMesh("Resources/Sample/SceneCreation/Models/Sphere.obj");
    Texture* ironTexture = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/albedo.png");
    Texture* ironMetalnessMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/metallic.png");
    Texture* ironRoughnessMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/roughness.png");
    Texture* ironAoMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/ao.png");

    for (int width = 0; width < 5; width++)
    {
        for (int height = 0; height < 5; height++)
        {
            for (int depth = 0; depth < 5; depth++)
            {
                Mathlib::Transform transform;
                transform.position = Mathlib::Vec3(-4.f + (width * 2.f), -4.f + (height * 2.f), (depth * 2.f));
                transform.scale = Mathlib::Vec3(0.02f, 0.02f, 0.02f);

                GameObjectCreateInfo createinfo;
                createinfo.transform = transform;
                createinfo.parent = nullptr;
                createinfo.name = "object_" + std::to_string(width * 5 + height);

                GameObject* obj = scene->AddGameObject(createinfo);

                MeshRenderer* meshRenderer = obj->GetComponent<MeshRenderer>();
                meshRenderer->mesh = sphere;
                meshRenderer->texture = ironTexture;
                meshRenderer->metalnessMap = ironMetalnessMap;
                meshRenderer->roughnessMap = ironRoughnessMap;
                meshRenderer->ambientOcclusionMap = ironAoMap;
                meshRenderer->fragmentShader = resourceManager->LoadShader("Resources/Engine/Shaders/PBRFragmentShader.spv", FRAGMENT);
                meshRenderer->shininess = 32.0f;
                meshRenderer->ambient = Mathlib::Vec3(0.1f, 0.1f, 0.1f);
                meshRenderer->diffuse = Mathlib::Vec3(0.5f, 0.5f, 0.5f);
                meshRenderer->specular = Mathlib::Vec3(1.0f, 1.0f, 1.0f);

                RotatorComponent* rotator = obj->AddComponent<RotatorComponent>();
                rotator->rotationAxis = ROTATION_AXIS::Y;
            }
        }
    }

    /* light 2 */

    Mathlib::Transform transform2;
    transform2.position = Mathlib::Vec3(-5.5f, 0.0f, -1.f);
    transform2.scale = Mathlib::Vec3(0.1f, 0.1f, 0.1f);

    GameObjectCreateInfo createinfo2;
    createinfo2.transform = transform2;
    createinfo2.parent = nullptr;
    createinfo2.name = "light_2";

    GameObject* light2 = scene->AddGameObject(createinfo2);

    PointLight* lightComponent = light2->AddComponent<PointLight>();
    lightComponent->color = Mathlib::Vec3(1.0f, 0.00f, 0.0f);
    lightComponent->range = 20.f;
    lightComponent->intensity = 3.f;

    MeshRenderer* meshRenderer1 = light2->GetComponent<MeshRenderer>();
    meshRenderer1->fragmentShader = resourceManager->LoadShader("Resources/Engine/Shaders/TextureFragmentShader.spv", FRAGMENT);

    ///* light 3 */

    //Mathlib::Transform transform3;
    //transform3.position = Mathlib::Vec3(5.5f, 0.0f, -1.f);
    //transform3.scale = Mathlib::Vec3(0.1f, 0.1f, 0.1f);

    //GameObjectCreateInfo createinfo3;
    //createinfo3.transform = transform3;
    //createinfo3.parent = nullptr;
    //createinfo3.name = "light_3";

    //GameObject* light3 = scene->AddGameObject(createinfo3);

    //PointLight* lightComponent3 = light3->AddComponent<PointLight>();
    //lightComponent3->color = Mathlib::Vec3(0.0f, 1.0f, 0.0f);
    //lightComponent3->range = 25.f;

    //MeshRenderer* meshRenderer3 = light3->GetComponent<MeshRenderer>();
    //meshRenderer3->fragmentShaderFilePath = "Resources/Engine/Shaders/TextureFragmentShader.spv";

    ///* light 4 */

    //Mathlib::Transform transform4;
    //transform4.position = Mathlib::Vec3(0.f, 0.0f, -5.f);
    //transform4.scale = Mathlib::Vec3(0.1f, 0.1f, 0.1f);

    //GameObjectCreateInfo createinfo4;
    //createinfo4.transform = transform4;
    //createinfo4.parent = nullptr;
    //createinfo4.name = "light_4";

    //GameObject* light4 = scene->AddGameObject(createinfo4);

    //DirectionalLight* lightComponent4 = light4->AddComponent<DirectionalLight>();
    //lightComponent4->color = Mathlib::Vec3(1.0f, 1.0f, 1.f);

    //MeshRenderer* meshRenderer4 = light4->GetComponent<MeshRenderer>();
    //meshRenderer4->enable = false;
    //meshRenderer4->fragmentShaderFilePath = "Resources/Engine/Shaders/TextureFragmentShader.spv";

    ///* light 5 */

    //Mathlib::Transform transform5;
    //transform5.position = Mathlib::Vec3(0.f, 0.0f, -5.f);
    //transform5.scale = Mathlib::Vec3(0.1f, 0.1f, 0.1f);

    //GameObjectCreateInfo createinfo5;
    //createinfo5.transform = transform5;
    //createinfo5.parent = nullptr;
    //createinfo5.name = "light_5";

    //GameObject* light5 = scene->AddGameObject(createinfo5);

    //SpotLight* lightComponent5 = light5->AddComponent<SpotLight>();
    //lightComponent5->color = Mathlib::Vec3(1.0f, 1.0f, 1.0f);
    //lightComponent5->range = 30.f;
    //lightComponent5->cutOff = 25.f;

    //MeshRenderer* meshRenderer5 = light5->GetComponent<MeshRenderer>();
    //meshRenderer5->enable = false;
    //meshRenderer5->fragmentShaderFilePath = "Resources/Engine/Shaders/TextureFragmentShader.spv";

    return scene;
}

Scene* SetupSphereScene()
{
    Scene* scene = sceneManager->AddScene();
    scene->name = "test_scene_simple_cube";

    Camera* camera = scene->GetCamera();
    Mathlib::Transform cameraTransform;
    cameraTransform.position = Mathlib::Vec3(0.0f, 0.0f, -2.f);
    cameraTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 180.f, 0.f));
    camera->SetLocalTransform(cameraTransform);
    camera->fov = 90.f;

    Mesh* sphere = resourceManager->LoadMesh("Resources/Sample/SceneCreation/Models/Sphere.obj");
    Shader* fragShader = resourceManager->LoadShader("Resources/Engine/Shaders/PBRFragmentShader.frag.spv", FRAGMENT);
    Texture* wallTexture = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/albedo.png");
    Texture* wallMetalnessMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/metallic.png");
    Texture* wallRoughnessMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/roughness.png");
    Texture* wallAoMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/ao.png");

    /*Cube 1*/
    Mathlib::Transform transform;
    transform.position = Mathlib::Vec3(-1.f, 0.f, 0.0f);
    transform.scale = Mathlib::Vec3(0.015f, 0.015f, 0.015f);

    GameObjectCreateInfo createinfo;
    createinfo.transform = transform;
    createinfo.parent = nullptr;
    createinfo.name = "sphere_1";

    GameObject* obj = scene->AddGameObject(createinfo);

    MeshRenderer* meshRenderer = obj->GetComponent<MeshRenderer>();
    meshRenderer->mesh = sphere;
    meshRenderer->texture = wallTexture;
    meshRenderer->metalnessMap = wallMetalnessMap;
    meshRenderer->roughnessMap = wallRoughnessMap;
    meshRenderer->ambientOcclusionMap = wallAoMap;
    meshRenderer->fragmentShader = fragShader;
    meshRenderer->shininess = 32.0f;
    meshRenderer->ambient = Mathlib::Vec4(0.1f, 0.1f, 0.1f, 1.f);
    meshRenderer->diffuse = Mathlib::Vec4(0.4f, 0.4f, 0.4f, 1.f);
    meshRenderer->specular = Mathlib::Vec4(0.8f, 0.8f, 0.8f, 1.f);

    /*RotatorComponent* rotator = obj->AddComponent<RotatorComponent>();
    rotator->rotationAxis = ROTATION_AXIS::Y;*/

    /*cube 2*/

    Mathlib::Transform transform2;
    transform2.position = Mathlib::Vec3(0.f, 0.f, 0.0f);
    transform2.scale = Mathlib::Vec3(0.015f, 0.015f, 0.015f);

    GameObjectCreateInfo createinfo2;
    createinfo2.transform = transform2;
    createinfo2.parent = nullptr;
    createinfo2.name = "sphere_2";

    GameObject* obj2 = scene->AddGameObject(createinfo2);

    Texture* ironTexture = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/albedo.png");
    Texture* ironMetalnessMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/metallic.png");
    Texture* ironRoughnessMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/roughness.png");
    Texture* ironAoMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/ao.png");

    MeshRenderer* meshRenderer2 = obj2->GetComponent<MeshRenderer>();
    meshRenderer2->mesh = sphere;
    meshRenderer2->texture = ironTexture;
    meshRenderer2->metalnessMap = ironMetalnessMap;
    meshRenderer2->roughnessMap = ironRoughnessMap;
    meshRenderer2->ambientOcclusionMap = ironAoMap;
    meshRenderer2->fragmentShader = fragShader;
    meshRenderer2->shininess = 32.0f;
    meshRenderer2->ambient = Mathlib::Vec3(0.1f, 0.1f, 0.1f);
    meshRenderer2->diffuse = Mathlib::Vec3(0.5f, 0.5f, 0.5f);
    meshRenderer2->specular = Mathlib::Vec3(1.0f, 1.0f, 1.0f);

    /*RotatorComponent* rotator2 = obj2->AddComponent<RotatorComponent>();
    rotator2->rotationAxis = ROTATION_AXIS::Y;*/

    /*cube 3*/

    Mathlib::Transform transform3;
    transform3.position = Mathlib::Vec3(1.f, 0.f, 0.0f);
    transform3.scale = Mathlib::Vec3(0.015f, 0.015f, 0.015f);

    GameObjectCreateInfo createinfo3;
    createinfo3.transform = transform3;
    createinfo3.parent = nullptr;
    createinfo3.name = "sphere_3";

    GameObject* obj3 = scene->AddGameObject(createinfo3);

    Texture* goldTexture = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Gold/albedo.png");
    Texture* goldMetalnessMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Gold/metallic.png");
    Texture* goldRoughnessMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Gold/roughness.png");
    Texture* goldAoMap = resourceManager->LoadTexture("Resources/Sample/SceneCreation/Textures/Gold/ao.png");

    MeshRenderer* meshRenderer3 = obj3->GetComponent<MeshRenderer>();
    meshRenderer3->mesh = sphere;
    meshRenderer3->texture = goldTexture;
    meshRenderer3->metalnessMap = goldMetalnessMap;
    meshRenderer3->roughnessMap = goldRoughnessMap;
    meshRenderer3->ambientOcclusionMap = goldAoMap;
    meshRenderer3->fragmentShader = fragShader;
    meshRenderer3->shininess = 32.0f;
    meshRenderer3->ambient = Mathlib::Vec3(0.1f, 0.1f, 0.1f);
    meshRenderer3->diffuse = Mathlib::Vec3(0.5f, 0.5f, 0.5f);
    meshRenderer3->specular = Mathlib::Vec3(1.0f, 1.0f, 1.0f);

    /*RotatorComponent* rotator3 = obj3->AddComponent<RotatorComponent>();
    rotator3->rotationAxis = ROTATION_AXIS::Y;*/

    /*light 1*/
    Mathlib::Transform transform4;
    transform4.position = Mathlib::Vec3(0.f, 0.0f, -2.f);
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

    /*RotatorComponent* rotator2 = scene->GetCamera()->AddComponent<RotatorComponent>();
    rotator2->rotationAxis = ROTATION_AXIS::Y;*/

    return scene;
}

void MainLoop()
{
    Scene* scene = SetupSphereScene();

    scene->Initialize();
    scene->Start();

    while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
        glfwPollEvents();
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