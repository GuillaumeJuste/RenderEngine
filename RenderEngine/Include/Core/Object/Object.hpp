#pragma once

#ifndef RENDERENGINE_OBJECT
#define RENDERENGINE_OBJECT

#include <string>

namespace RenderEngine::Core
{
    /**
     * @brief Scene Object base class
    */
    class Object
    {
    private :
        /**
         * @brief value of the next object created UID
        */
        static unsigned int nextId;
    protected :
        /**
         * @brief object unique ID
        */
        unsigned int uid;

        /**
         * @brief object name
        */
        std::string name;

        /**
         * @brief should the object be displayed
        */
        bool isHidden = false;
        
    public :
        Object();
        ~Object() = default;

        /**
         * @brief Get object UID
         * @return object UID
        */
        unsigned int GetUId() const;

        /**
         * @brief Get object name
         * @return object name
        */
        std::string GetName() const;

        /**
         * @brief Get if object is hidden
         * @return true if object is hidden
        */
        bool GetIsHidden() const;

        /**
         * @brief set object name
         * @param _name new object name
        */
        void SetName(std::string _name);

        /**
         * @brief set if object should be visible
         * @param _val object new visibility value
        */
        void SetIsHidden(bool _val);

        /**
         * @brief compare two objects
         * @param _rhs object to compare to this
         * @return true if objects are identical
        */
        bool operator==(const Object& _rhs);
    };
}

#endif