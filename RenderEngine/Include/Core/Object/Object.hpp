#pragma once

#ifndef RENDERENGINE_OBJECT
#define RENDERENGINE_OBJECT

#include <string>

namespace RenderEngine::Core
{
    class Object
    {
    private :
        static unsigned int nextId;
    protected :
        unsigned int uid;
        std::string name;
        
        
    public :
        Object();
        ~Object() = default;

        unsigned int GetUId();
        std::string GetName();
        void SetName(std::string _name);

        bool operator==(const Object& _rhs);
    };
}

#endif