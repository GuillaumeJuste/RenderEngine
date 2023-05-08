#include "ResourceManager/Wrapper/StbiWrapper.hpp"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace RenderEngine::Wrapper;

bool StbiWrapper::LoadTexture(const std::string& _filePath, Texture& _output)
{
	int texWidth, texHeight, texChannels;

	char* data = reinterpret_cast<char*>(stbi_load(_filePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha));

	if (!data)
	{
		return false;
	}

	_output.width = texWidth;
	_output.height = texHeight;

	_output.imageSize = _output.width * _output.height * 4;
	_output.filePath = _filePath;

	_output.pixels = reinterpret_cast<char*>(stbi__malloc(_output.imageSize));
	std::memmove(_output.pixels, data, _output.imageSize * sizeof(char));

	stbi_image_free(data);

	return true;
}