#pragma once

#ifndef RENDERENGINE_GAMEOBJECT
#define RENDERENGINE_GAMEOBJECT

#include "SceneGraph/Object/Object.hpp"
#include "SceneGraph/Components/Component.hpp"
#include "SceneGraph/Components/Transfom/Transform.hpp"
#include "SceneGraph/Object/GameObject/GameObjectCreateInfo.hpp"

#include "Utils/Event/Event.hpp"

#include <vector>

namespace RenderEngine::SceneGraph
{
    /**
     * @brief GameObject class implementation
    */
    class GameObject : public Object
    {
    protected:

        /**
         * @brief GameObject Parent
        */
        GameObject* parent = nullptr;

        /**
         * @brief object transform
        */
        Transform transform;

        /**
         * @brief GameObject childrens
        */
        std::vector<GameObject*> childrens;

        /**
         * @brief GameObject components
        */
        std::vector<Component*> components;

    public:
        /**
         * @brief event called when a component is added to the GameObject
        */
        RenderEngine::Utils::Event<Component*> ComponentAdded;

        /**
         * @brief event called when a component is removed from the GameObject
        */
        RenderEngine::Utils::Event<Component*> ComponentRemoved;

        /// default constructor
        GameObject() = default;

        ///default destructor
        ~GameObject() = default;

        /**
         * @brief Function to initialize GameObject
         * @param _createinfo informations to initialize GAmeObject from
         * @param _output GAmeObject to initialize
        */
        static void InitializeGameObject(const GameObjectCreateInfo& _createinfo, GameObject* _output);

        /**
         * @brief initialize GameObject
        */
        void Initialize();

        /**
         * @brief start GameObject
        */
        void Start();

        /**
         * @brief Update Gameobject
         * @param _deltaTime frame delta time
        */
        void Update(double _deltaTime);

        /**
         * @brief GameObject fixed update
         * @param _deltaTime fixed delta time
        */
        void FixedUpdate(double _deltaTime);

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
         * @brief Add a component of type <T> to the game object
         * @tparam T Type of the component
         * @return component added to the object
        */
        template<typename T>
        T* AddComponent();

        /**
         * @brief Get the first component of type <T>
         * @tparam T Type of the component
         * @return pointer to the component found
        */
        template<typename T>
        T* GetComponent();

        /**
         * @brief Get all the components of type <T>
         * @tparam T Type of the component
         * @return vector containing pointer to all the matching components
        */
        template<typename T>
        std::vector<T*> GetComponents();

        /**
         * @brief remove the first component of type <T>
         * @tparam T T Type of the component
         * @param _component pointer to the component to remove
         * @return true if the component was properly removed
        */
        template<typename T>
        bool RemoveComponent(T* _component);

        /**
         * @brief Get GameObject parent
         * @return GameObject parent
        */
        GameObject* GetParent() const;

        /**
         * @brief Get GameObject world transform
        */
        Mathlib::Transform GetWorldTransform() const;

        /**
         * @brief Get the game object local transform
        */
        Mathlib::Transform GetLocalTransform() const;

        /**
         * @brief Set the game object local transform
         * @param _newTransform new transform to apply to the GameObject
        */
        void SetLocalTransform(Mathlib::Transform _newTransform);


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

        /**
         * @brief clear the GameObject
        */
        void Cleanup();
    };
}

#include "SceneGraph/Object/GameObject/GameObject.inl"

#endif