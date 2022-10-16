#include "Core/Components/Material/Material.hpp"

using namespace RenderEngine::Core;

void Material::Initialize()
{

}

void Material::Start()
{

}

void Material::Update()
{

}

Texture* Material::GetTexture()
{
	return texture;
}

void Material::SetTexture(Texture* _texture)
{
	texture = _texture;
}