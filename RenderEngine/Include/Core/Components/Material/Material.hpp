#pragma once

#ifndef RENDERENGINE_MATERIAL
#define RENDERENGINE_MATERIAL

#include "Core/Components/Component.hpp"
#include "Core/RessourceManager/Texture.hpp"


namespace RenderEngine::Core
{
    /**
     * @brief Component class implementation
    */
    class Material : public Component
    {
    private:
        Texture* texture;
    public:
        Material() = default;
        ~Material() = default;

        void Initialize();
        void Start();
        void Update();

        Texture* GetTexture();

        void SetTexture(Texture* _texture);
    };
}

#endif