#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "Engine/Vulkan/Misc/VulkanBaseInclude.hpp"
#include "Engine/Vulkan/VulkanContext/VulkanContext.hpp"
#include "Window/Glfw/Window.hpp"
#include "Core/Scene/SceneManager.hpp"
#include "Core/RessourceManager/RessourceManager.hpp"
#include "Core/Object/GameObject/GameObject.hpp"
#include "Core/Components/MeshRenderer/MeshRenderer.hpp"
#include "Core/Components/Light/PointLight.hpp"
#include "Core/Components/Light/DirectionalLight.hpp"
#include "Core/Components/Light/SpotLight.hpp"
#include "Component/RotatorComponent.hpp"
#include <iostream>

#include<Mathlib/Mathlib/Include/Misc/Math.hpp>

using namespace RenderEngine::Engine::Base;
using namespace RenderEngine::Core;

/**
* @brief Window for engine rendering
*/
GLFW::Window* window;

/**
 * @brief vulkan context
*/
RenderEngine::Engine::Vulkan::VulkanContext vulkanContext;

/**
 * @brief device used to render scene
*/
IDeviceContext* deviceContext;

/**
 * @brief render context to render scene
*/
IRenderContext* renderContext;

SceneManager sceneManager;

RessourceManager* ressourceManager;

using namespace RenderEngine;
using namespace RenderEngine::Engine::Vulkan;
using namespace RenderEngine::Core;
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


Scene* SetupTestScene()
{
    Scene* scene = sceneManager.AddScene();
    scene->name = "test_scene_1";

    Camera* camera = scene->GetCamera();
    Mathlib::Transform cameraTransform;
    cameraTransform.position = Mathlib::Vec3(0.0f, 0.0f, -10.0f);
    camera->SetLocalTransform(cameraTransform);

    Mesh* mesh = RessourceManager::GetInstance()->LoadMesh("Resources/Sample/SceneCreation/Models/viking_room.obj");

    Texture* texture = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/viking_room.png");
    Texture* texture2 = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/texture.jpg");

    Mathlib::Transform transform;
    transform.position = Mathlib::Vec3(0.f, .0f, 0.f);
    transform.scale = Mathlib::Vec3(1.f, 1.f, 1.f);
    transform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 90.f, 90.f));

    GameObjectCreateInfo createinfo;
    createinfo.transform = transform;
    createinfo.parent = nullptr;
    createinfo.name = "first_object";

    GameObject* obj = scene->AddGameObject(createinfo);

    MeshRenderer* meshRenderer = obj->GetComponent<MeshRenderer>();
    meshRenderer->mesh = mesh;
    meshRenderer->texture = texture;
    meshRenderer->fragmentShaderFilePath = "Resources/Engine/Shaders/TextureFragmentShader.frag.spv";

    Mathlib::Transform transform2;
    transform2.position = Mathlib::Vec3(2.f, 3.0f, 2.f);
    transform2.scale = Mathlib::Vec3(0.5f, 0.5f, 0.5f);

    GameObjectCreateInfo createinfo2;
    createinfo2.transform = transform2;
    createinfo2.parent = nullptr;
    createinfo2.name = "second_object";

    GameObject* obj2 = scene->AddGameObject(createinfo2);
    MeshRenderer* meshRenderer2 = obj2->GetComponent<MeshRenderer>();
    meshRenderer2->texture = texture2;

    RotatorComponent* rotator2 = obj2->AddComponent <RotatorComponent>();
    rotator2->rotationAxis = ROTATION_AXIS::Z;


    /* Mathlib::Transform transform4;
    transform4.position = Mathlib::Vec3(1.5f, 0.0f, 0.f);
    transform4.scale = Mathlib::Vec3(0.5f, 0.5f, 0.5f);

    GameObjectCreateInfo createinfo4;
    createinfo4.transform = transform4;
    createinfo4.parent = obj2;
    createinfo4.name = "fourth_object";

    GameObject* obj4 = scene->AddGameObject(createinfo4);*/


    return scene;
}

Scene* SetupIlluminationScene()
{
    Scene* scene = sceneManager.AddScene();
    scene->name = "test_scene_illumination";

    Camera* camera = scene->GetCamera();
    Mathlib::Transform cameraTransform;
    cameraTransform.position = Mathlib::Vec3(0.0f, 0.0f, -6.0f);
    cameraTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));
    camera->SetLocalTransform(cameraTransform);
    camera->fov = 90.f;

    Texture* texture = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/container.png");
    Texture* specularMap = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/container_specular.png");

    Mesh* sphere = RessourceManager::GetInstance()->LoadMesh("Resources/Sample/SceneCreation/Models/Sphere.obj");
    Texture* ironTexture = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/albedo.png");
    Texture* ironMetalnessMap = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/metallic.png");
    Texture* ironRoughnessMap = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/roughness.png");
    Texture* ironAoMap = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/ao.png");

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
                meshRenderer->fragmentShaderFilePath = "Resources/Engine/Shaders/PBRFragmentShader.spv";
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
    meshRenderer1->fragmentShaderFilePath = "Resources/Engine/Shaders/TextureFragmentShader.spv";

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

Scene* SetupSimplePlaneScene()
{
    Scene* scene = sceneManager.AddScene();
    scene->name = "test_scene_simple_plane";

    Camera* camera = scene->GetCamera();
    Mathlib::Transform cameraTransform;
    cameraTransform.position = Mathlib::Vec3(0.0f, 2.0f, -20.0f);
    cameraTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));
    camera->SetLocalTransform(cameraTransform);
    camera->fov = 90.f;

    Mathlib::Transform objTransform;
    objTransform.position = Mathlib::Vec3(0.f, -2.f, 0.0f);
    objTransform.scale = Mathlib::Vec3(15.f, 0.0f, 15.f);
    objTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));

    GameObjectCreateInfo objCreateinfo;
    objCreateinfo.transform = objTransform;
    objCreateinfo.parent = nullptr;

    GameObject* obj = scene->AddGameObject(objCreateinfo);

    MeshRenderer* objMeshRenderer = obj->GetComponent<MeshRenderer>();
    objMeshRenderer->shininess = 32.0f;
    //meshRenderer->fragmentShaderFilePath = "Resources/Engine/Shaders/TextureFragmentShader.spv";
    objMeshRenderer->texture = RessourceManager::GetInstance()->LoadTexture("Resources/Textures/Red.jpg");

    Mathlib::Transform obj2Transform;
    obj2Transform.position = Mathlib::Vec3(0.f, 13.f, 15.0f);
    obj2Transform.scale = Mathlib::Vec3(15.f, 15.0f, 0.f);
    obj2Transform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));

    GameObjectCreateInfo obj2Createinfo;
    obj2Createinfo.transform = obj2Transform;
    obj2Createinfo.parent = nullptr;

    GameObject* obj2 = scene->AddGameObject(obj2Createinfo);

    MeshRenderer* obj2MeshRenderer = obj2->GetComponent<MeshRenderer>();
    obj2MeshRenderer->shininess = 32.0f;
    //meshRenderer->fragmentShaderFilePath = "Resources/Engine/Shaders/TextureFragmentShader.spv";
    obj2MeshRenderer->texture = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/Red.jpg");

    Mathlib::Transform lightTransform;
    lightTransform.position = Mathlib::Vec3(0.f, 0.0f, -15.f);
    lightTransform.scale = Mathlib::Vec3(0.1f, 0.1f, 0.1f);
    lightTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(30.f, 0.f, 0.f));

    GameObjectCreateInfo lightCreateInfo;
    lightCreateInfo.transform = lightTransform;
    lightCreateInfo.parent = nullptr;
    lightCreateInfo.name = "light_1";

    GameObject* light1 = scene->AddGameObject(lightCreateInfo);

    SpotLight* lightComponent = light1->AddComponent<SpotLight>();
    lightComponent->color = Mathlib::Vec3(1.f, 1.0f, 1.0f);
    lightComponent->range = 30.f;
    lightComponent->cutOff = 12.5f;

    return scene;
}

Scene* SetupSimpleCubeScene()
{
    Scene* scene = sceneManager.AddScene();
    scene->name = "test_scene_simple_cube";

    Camera* camera = scene->GetCamera();
    Mathlib::Transform cameraTransform;
    cameraTransform.position = Mathlib::Vec3(0.0f, 0.0f, -2.f);
    cameraTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, 0.f));
    camera->SetLocalTransform(cameraTransform);
    camera->fov = 90.f;

    Mesh* sphere = RessourceManager::GetInstance()->LoadMesh("Resources/Sample/SceneCreation/Models/Sphere.obj");
    Texture* wallTexture = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/albedo.png");
    Texture* wallMetalnessMap = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/metallic.png");
    Texture* wallRoughnessMap = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/roughness.png");
    Texture* wallAoMap = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/Wall/ao.png");

    /*Cube 1*/
    Mathlib::Transform transform;
    transform.position = Mathlib::Vec3(-1.f, 0.f, 0.0f);
    transform.scale = Mathlib::Vec3(0.02f, 0.02f, 0.02f);

    GameObjectCreateInfo createinfo;
    createinfo.transform = transform;
    createinfo.parent = nullptr;
    createinfo.name = "Cube_1";

    GameObject* obj = scene->AddGameObject(createinfo);

    MeshRenderer* meshRenderer = obj->GetComponent<MeshRenderer>();
    meshRenderer->mesh = sphere;
    meshRenderer->texture = wallTexture;
    meshRenderer->metalnessMap = wallMetalnessMap;
    meshRenderer->roughnessMap = wallRoughnessMap;
    meshRenderer->ambientOcclusionMap = wallAoMap;
    meshRenderer->fragmentShaderFilePath = "Resources/Engine/Shaders/BlinnPhongFragmentShader.frag.spv";
    meshRenderer->shininess = 32.0f;
    meshRenderer->ambient = Mathlib::Vec4(0.1f, 0.1f, 0.1f, 1.f);
    meshRenderer->diffuse = Mathlib::Vec4(0.4f, 0.4f, 0.4f, 1.f);
    meshRenderer->specular = Mathlib::Vec4(0.8f, 0.8f, 0.8f, 1.f);

    /*RotatorComponent* rotator = obj->AddComponent<RotatorComponent>();
    rotator->rotationAxis = ROTATION_AXIS::Y;*/

    /*cube 2*/

    Mathlib::Transform transform2;
    transform2.position = Mathlib::Vec3(1.f, 0.f, 0.0f);
    transform2.scale = Mathlib::Vec3(0.02f, 0.02f, 0.02f);

    GameObjectCreateInfo createinfo2;
    createinfo2.transform = transform2;
    createinfo2.parent = nullptr;
    createinfo2.name = "Cube_2";

    GameObject* obj2 = scene->AddGameObject(createinfo2);

    Texture* ironTexture = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/albedo.png");
    Texture* ironMetalnessMap = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/metallic.png");
    Texture* ironRoughnessMap = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/roughness.png");
    Texture* ironAoMap = RessourceManager::GetInstance()->LoadTexture("Resources/Sample/SceneCreation/Textures/Rusted_iron/ao.png");

    MeshRenderer* meshRenderer2 = obj2->GetComponent<MeshRenderer>();
    meshRenderer2->mesh = sphere;
    meshRenderer2->texture = ironTexture;
    meshRenderer2->metalnessMap = ironMetalnessMap;
    meshRenderer2->roughnessMap = ironRoughnessMap;
    meshRenderer2->ambientOcclusionMap = ironAoMap;
    meshRenderer2->fragmentShaderFilePath = "Resources/Engine/Shaders/BlinnPhongFragmentShader.frag.spv";
    meshRenderer2->shininess = 32.0f;
    meshRenderer2->ambient = Mathlib::Vec3(0.1f, 0.1f, 0.1f);
    meshRenderer2->diffuse = Mathlib::Vec3(0.5f, 0.5f, 0.5f);
    meshRenderer2->specular = Mathlib::Vec3(1.0f, 1.0f, 1.0f);

    /*RotatorComponent* rotator2 = obj2->AddComponent<RotatorComponent>();
    rotator2->rotationAxis = ROTATION_AXIS::Y;*/

    /*light 1*/
    Mathlib::Transform transform4;
    transform4.position = Mathlib::Vec3(0.f, 0.0f, -1.f);
    transform4.scale = Mathlib::Vec3(0.01f, 0.01f, 0.01f);

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
    meshRenderer4->fragmentShaderFilePath = "Resources/Engine/Shaders/TextureFragmentShader.frag.spv";

    return scene;
}

void MainLoop()
{
    Scene* scene = SetupSimpleCubeScene();

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
    vulkanContext.Cleanup();

    window->Cleanup();
    delete window;
}

void Run()
{
    InitWindow();
    ressourceManager = RessourceManager::GetInstance();
    InitEngine();
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