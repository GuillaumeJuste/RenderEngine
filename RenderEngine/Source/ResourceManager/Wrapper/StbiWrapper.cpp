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

	_output.channels = 4;

	_output.imageSize = _output.width * _output.height * _output.channels;

	return true;
}

bool StbiWrapper::LoadCubemap(const CubemapImportInfos& _importInfos, RawCubemap& _output)
{
	char* data[6]{};

	// Load textures.
	for (size_t i = 0; i < 6; ++i)
	{
		data[i] = reinterpret_cast<char*>(stbi_load(_importInfos.pathes[i].c_str(),
			&_output.width,
			&_output.height,
			&_output.channels,
			STBI_rgb_alpha
		));

		if (!data[i])
		{
			return false;
		}
	}

	_output.channels = 4;

	_output.imageSize = _output.width * _output.height * _output.channels;

	_output.pixels = reinterpret_cast<char*>(stbi__malloc(6u * _output.imageSize));

	for (size_t i = 0; i < 6; ++i)
	{
		std::memmove(_output.pixels + i * _output.imageSize, data[i], _output.imageSize);

		FreeImage(data[i]);
	}
	return true;
}

void StbiWrapper::FreeImage(char* image)
{
	stbi_image_free(image);
}