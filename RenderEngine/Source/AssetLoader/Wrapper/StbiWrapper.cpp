#include "AssetLoader/Wrapper/StbiWrapper.hpp"

#include <stdexcept>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace Wrapper;

bool StbiWrapper::LoadTexture(const std::string& _filePath, bool _isHDR, RawTexture& _output)
{
	int channels = 0.f;
	char* data;
	if (_isHDR)
	{
		data = reinterpret_cast<char*>(stbi_loadf(_filePath.c_str(), &_output.width, &_output.height, &channels, STBI_rgb_alpha));
	}
	else
	{
		data = reinterpret_cast<char*>(stbi_load(_filePath.c_str(), &_output.width, &_output.height, &channels, STBI_rgb_alpha));
	}
	
	if (!data)
	{
		return false;
	}

	_output.isHdr = _isHDR;

	_output.channels = 4;

	_output.imageSize = _output.width * _output.height * _output.channels;
	if (_isHDR)
		_output.imageSize *= sizeof(float);

	_output.data.clear();
	_output.data.insert(_output.data.end(), &data[0], &data[_output.imageSize]);

	_output.imageCount = 1;

	return true;
}

bool StbiWrapper::LoadCubemap(const CubemapImportInfos& _importInfos, RawTexture& _output)
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

	for (size_t i = 0; i < 6; ++i)
	{
		_output.data.insert(_output.data.end(), &data[i][0], &data[i][_output.imageSize]);

		FreeImage(data[i]);
	}

	_output.imageCount = 6;

	return true;
}

void StbiWrapper::FreeImage(char* image)
{
	stbi_image_free(image);
}

void StbiWrapper::FreeImage(float* image)
{
	stbi_image_free(image);
}