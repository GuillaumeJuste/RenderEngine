#include "AssetLoader/Wrapper/StbiWrapper.hpp"

#include <stdexcept>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace Wrapper;

int StbiWrapper::GetSTBIFormat(TextureFormat _format)
{
	int formatToInt = (int)_format;
	return formatToInt - (formatToInt / (int)TextureFormat::HDR_GREY) * (int)TextureFormat::HDR_GREY + 1;
}

bool StbiWrapper::LoadTexture(const std::string& _filePath, TextureFormat _format, RawTexture& _output)
{
	int channels = 0;

	_output.isHdr = _format >= TextureFormat::HDR_GREY ? true : false;
	int stbiFormat = GetSTBIFormat(_format);

	char* data;
	if (_output.isHdr)
	{
		data = reinterpret_cast<char*>(stbi_loadf(_filePath.c_str(), &_output.width, &_output.height, &channels, stbiFormat));
	}
	else
	{
		data = reinterpret_cast<char*>(stbi_load(_filePath.c_str(), &_output.width, &_output.height, &channels, stbiFormat));
	}
	
	if (!data)
	{
		return false;
	}
	
	_output.format = _format;
	_output.channels = stbiFormat;

	_output.imageSize = _output.width * _output.height * _output.channels;
	if (_output.isHdr)
		_output.imageSize *= sizeof(float);

	_output.data.clear();
	_output.data.insert(_output.data.end(), &data[0], &data[_output.imageSize]);
	stbi_image_free(data);

	_output.imageCount = 1;

	return true;
}

bool StbiWrapper::LoadCubemap(const CubemapImportInfos& _importInfos, TextureFormat _format, RawTexture& _output)
{
	_output.isHdr = _format >= TextureFormat::HDR_GREY ? true : false;
	int stbiFormat = GetSTBIFormat(_format);

	int channels = 0;

	char* data[6]{};
	// Load textures.
	for (size_t i = 0; i < 6; ++i)
	{
		if (_output.isHdr)
		{
			data[i] = reinterpret_cast<char*>(stbi_loadf(_importInfos.pathes[i].c_str(), &_output.width, &_output.height, &channels, stbiFormat));
		}
		else
		{
			data[i] = reinterpret_cast<char*>(stbi_load(_importInfos.pathes[i].c_str(), &_output.width, &_output.height, &channels, stbiFormat));
		}

		if (!data[i])
		{
			return false;
		}
	}

	_output.channels = stbiFormat;

	_output.imageSize = _output.width * _output.height * _output.channels;
	if (_output.isHdr)
		_output.imageSize *= sizeof(float);

	for (size_t i = 0; i < 6; ++i)
	{
		_output.data.insert(_output.data.end(), &data[i][0], &data[i][_output.imageSize]);

		FreeImage(data[i]);
	}

	_output.imageCount = 6;
	_output.format = _format;

	return true;
}

void StbiWrapper::FreeImage(char* image)
{
	stbi_image_free(image);
}

