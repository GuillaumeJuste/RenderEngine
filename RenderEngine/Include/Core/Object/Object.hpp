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
        bool isHidden = false;
        
    public :
        Object();
        ~Object() = default;

        unsigned int GetUId() const;
        std::string GetName() const;
        bool GetIsHidden() const;
        void SetName(std::string _name);
        void SetIsHidden(bool _val);

        bool operator==(const Object& _rhs);
    };
}

#endif