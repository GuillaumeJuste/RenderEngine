#pragma once

#ifndef RENDERENGINE_GAMEOBJECT
#define RENDERENGINE_GAMEOBJECT

#include "Core/Object/Object.hpp"
#include "Core/Object/Mesh/Mesh.hpp"
#include "Core/Object/GameObject/GameObjectCreateInfo.hpp"
#include "Transform/Transform.hpp"
#include <vector>

namespace RenderEngine::Core
{
    class GameObject : public Object
    {
    private:
        Mesh* mesh = nullptr;

        GameObject* parent = nullptr;

        std::vector<GameObject*> childrens;

        Mathlib::Transform transform;

    public:
        GameObject() = default;
        ~GameObject() = default;

        static void InitializeGameObject(const GameObjectCreateInfo& _createinfo, GameObject* _output);

        void AddChild(GameObject* _newChild);
        bool RemoveChild(GameObject* _child);

        Mesh* GetMesh() const;
        GameObject* GetParent() const;
        const Mathlib::Transform& GetTransform() const;
        Mathlib::Transform GetWorldTransform();

        std::vector<GameObject*> GetChildrens() const;

        void SetParent(GameObject* _newParent);
    };
}

#endif