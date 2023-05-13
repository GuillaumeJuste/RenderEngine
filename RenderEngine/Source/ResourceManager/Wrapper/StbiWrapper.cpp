#include "ResourceManager/Wrapper/StbiWrapper.hpp"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace RenderEngine::Wrapper;

bool StbiWrapper::LoadTexture(const std::string& _filePath, RawTexture& _output)
{
	_output.pixels = reinterpret_cast<char*>(stbi_load(_filePath.c_str(), &_output.width, &_output.height, &_output.channels, STBI_rgb_alpha));

	if (!_output.pixels)
	{
		return false;
	}

	_output.imageSize = _output.width * _output.height * 4;

	return true;
}

void StbiWrapper::FreeImage(char* image)
{
	stbi_image_free(image);
}