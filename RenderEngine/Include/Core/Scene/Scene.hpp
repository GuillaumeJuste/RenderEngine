#pragma once

#ifndef RENDERENGINE_SCENE
#define RENDERENGINE_SCENE

#include "Core/Object/GameObject/GameObject.hpp"
#include "Core/Object/GameObject/GameObjectCreateInfo.hpp"
#include "Core/Scene/SceneCreateInfo.hpp"

#include <string>
#include <vector>

namespace RenderEngine::Core
{
    class Scene
    {
    private:
        std::string name;
        std::vector<GameObject> gameObjects;
        GameObject rootObject;

    public:
        Scene() = default;
        ~Scene() = default;

        static void InitializeScene(const SceneCreateInfo& _createinfo, Scene* _output);

        GameObject* AddGameObject(GameObjectCreateInfo _createInfo);
        bool RemoveGameObject(GameObject* _gao);

        const GameObject* GetGameObjectByName(std::string _name) const;
        const GameObject* GetGameObjectByID(unsigned int _id) const;

        GameObject* GetSceneRoot();
    };
}

#endif