#include "ResourceManager/Assets/Shader/Shader.hpp"

using namespace RenderEngine::Assets;

Shader::~Shader()
{
	if (iShader != nullptr)
	{
		iShader->Clean();
		delete iShader;
	}
}