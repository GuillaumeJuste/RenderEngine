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

        Object* parent = nullptr;

        std::vector<Object*> childrens;

        Mathlib::Transform transform;

    public:
        GameObject() = default;
        ~GameObject() = default;

        static void InitializeGameObject(const GameObjectCreateInfo& _createinfo, GameObject* _output);

        void AddChild(Object* _newChild);
        bool RemoveChild(Object* _child);

        Mesh* GetMesh();
        Object* GetParent();
        const Mathlib::Transform& GetTransform() const;
        std::vector<Object*> GetChildrens();
    };
}

#endif