#include "ResourceManager/Wrapper/StbiWrapper.hpp"

#include <stdexcept>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace RenderEngine::Wrapper;

bool StbiWrapper::LoadTexture(const std::string& _filePath, bool _isHDR, bool _computeMipmap, RawTexture& _output)
{
	if (_isHDR)
	{
		_output.dataF = stbi_loadf(_filePath.c_str(), &_output.width, &_output.height, &_output.channels, STBI_rgb_alpha);
		_output.isHdr = true;

		if (!_output.dataF)
		{
			return false;
		}
	}
	else
	{
		_output.dataC = reinterpret_cast<char*>(stbi_load(_filePath.c_str(), &_output.width, &_output.height, &_output.channels, STBI_rgb_alpha));
		_output.isHdr = false;

		if (!_output.dataC)
		{
			return false;
		}
	}

	_output.channels = 4;

	_output.imageSize = _output.width * _output.height * _output.channels;
	if (_isHDR)
		_output.imageSize *= sizeof(float);

	if(_computeMipmap)
		_output.mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(_output.width, _output.height)))) + 1;

	_output.imageCount = 1;

	return true;
}

bool StbiWrapper::LoadCubemap(const CubemapImportInfos& _importInfos, bool _computeMipmap, RawTexture& _output)
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

	_output.dataC = reinterpret_cast<char*>(stbi__malloc(6u * _output.imageSize));

	if (_computeMipmap)
		_output.mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(_output.width, _output.height)))) + 1;

	for (size_t i = 0; i < 6; ++i)
	{
		std::memmove(_output.dataC + i * _output.imageSize, data[i], _output.imageSize);

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