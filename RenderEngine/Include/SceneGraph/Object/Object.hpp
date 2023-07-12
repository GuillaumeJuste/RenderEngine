#pragma once

#ifndef RENDERENGINE_OBJECT
#define RENDERENGINE_OBJECT

#include <string>

namespace RenderEngine::SceneGraph
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
    public :
        ///constructor
        Object();
        
        /// default destructor
        ~Object() = default;

        /**
        * @brief object name
        */
        std::string name;

        /**
         * @brief should the object be displayed
        */
        bool enable = true;

        /**
         * @brief Get object UID
         * @return object UID
        */
        unsigned int GetUId() const;

        /**
         * @brief compare two objects
         * @param _rhs object to compare to this
         * @return true if objects are identical
        */
        bool operator==(const Object& _rhs) const;
    };
}

#endif