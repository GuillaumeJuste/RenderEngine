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
    /**
     * @brief GameObject class
    */
    class GameObject : public Object
    {
    private:
        /**
         * @brief GameObject Mesh
        */
        Mesh* mesh = nullptr;

        /**
         * @brief GameObject Parent
        */
        GameObject* parent = nullptr;

        /**
         * @brief GameObject childrens
        */
        std::vector<GameObject*> childrens;

        /**
         * @brief GameObject transform
        */
        Mathlib::Transform transform;

    public:
        GameObject() = default;
        ~GameObject() = default;

        /**
         * @brief Function to initialize GameObject
         * @param _createinfo informations to initialize GAmeObject from
         * @param _output GAmeObject to initialize
        */
        static void InitializeGameObject(const GameObjectCreateInfo& _createinfo, GameObject* _output);

        /**
         * @brief Add child to GAmeObject
         * @param _newChild child to add to this GameObject
        */
        void AddChild(GameObject* _newChild);

        /**
         * @brief Remove child from this GameObject
         * @param _child child to remove from this GameObject
         * @return 
        */
        bool RemoveChild(GameObject* _child);

        /**
         * @brief Get GameObject Mesh
         * @return GameObject Mesh
        */
        Mesh* GetMesh() const;

        /**
         * @brief Get GameObject parent
         * @return GameObject parent
        */
        GameObject* GetParent() const;

        /**
         * @brief Get GameObject transform
         * @return GameObject transfom
        */
        const Mathlib::Transform& GetTransform() const;
        
        /**
         * @brief Get GameObject world transform
         * @return GameObject world transform
        */
        Mathlib::Transform GetWorldTransform();

        /**
        * @brief Get GameObject list of childrens
        * @return GameObject list of childrens
       */
        std::vector<GameObject*> GetChildrens() const;

        /**
         * @brief Set GameObject parent
         * @param _newParent new game object parent of this game object
        */
        void SetParent(GameObject* _newParent);
    };
}

#endif