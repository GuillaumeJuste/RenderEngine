#include "Rendering/Base/ResourceManager/ResourceManager.hpp"

#include <stdexcept>
#include <cstring>
#include <fstream>
#include <filesystem>

using namespace RenderEngine::Rendering;
using namespace Loader;

#define ASSETPATH "Resources/Engine/Assets/"

Mesh* ResourceManager::LoadMesh(std::string _filePath)
{
	Mesh* mesh = GetMesh(_filePath);
	if (mesh != nullptr)
		return mesh;

	RawMesh rawMesh = assetLoader.LoadMesh(_filePath);
	if (rawMesh.isValid)
	{
		Mesh* newMesh = new Mesh();
		newMesh->vertexBuffer = CreateVertexBufferObject(rawMesh);
		newMesh->indexBuffer = CreateIndexBufferObject(rawMesh);

		newMesh->filePath = _filePath;
		newMesh->indiceCount = rawMesh.indices.size();
		meshManager.Add(_filePath, newMesh);

		return newMesh;
	}

	return nullptr;
}

Mesh* ResourceManager::GetMesh(std::string _filePath)
{
	return meshManager.Get(_filePath);
}

bool ResourceManager::UnloadMesh(Mesh* _mesh)
{
	return meshManager.Unload(_mesh->filePath);
}

Texture* ResourceManager::LoadTexture(std::string _filePath, TextureFormat _format, bool _computeMipmap)
{
	Texture* texture = GetTexture(_filePath);
	if (texture != nullptr)
		return texture;

	RawTexture rawTexture = assetLoader.LoadTexture(_filePath, _format);
	if (rawTexture.isValid)
	{
		rawTexture.mipLevels = 1;
		if (_computeMipmap)
			rawTexture.mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(rawTexture.width, rawTexture.height)))) + 1;
		Texture* newTexture = new Texture();
		newTexture->iTexture = CreateTexture(rawTexture);
		newTexture->filePath = _filePath;
		newTexture->height = rawTexture.height;
		newTexture->width = rawTexture.width;
		newTexture->imageSize = rawTexture.imageSize;
		newTexture->mipLevels = rawTexture.mipLevels;
		newTexture->imageCount = 1;
		newTexture->isHDR = rawTexture.isHdr;
		textureManager.Add(_filePath, newTexture);

		return newTexture;
	}

	return nullptr;
}

Texture* ResourceManager::GetTexture(std::string _filePath)
{
	return textureManager.Get(_filePath);
}

bool ResourceManager::UnloadTexture(Texture* _texture)
{
	return textureManager.Unload(_texture->filePath);
}

Shader* ResourceManager::LoadShader(std::string _filePath, ShaderStage _shaderStage)
{
	Shader* shader = GetShader(_filePath);
	if (shader != nullptr)
		return shader;

	RawShader rawShader;
	std::filesystem::path filePath(_filePath);
	if (filePath.extension() == ".spv")
		rawShader = assetLoader.LoadShaderSPV(_filePath);
	else if(filePath.extension() == ".hlsl")
		rawShader = assetLoader.LoadShaderHLSL(_filePath, _shaderStage);

	if (rawShader.isValid)
	{
		Shader* newShader = new Shader();
		rawShader.stage = _shaderStage;
		newShader->iShader = CreateShader(rawShader);
		newShader->filePath = _filePath;
		shaderManager.Add(_filePath, newShader);

		return newShader;
	}

	return nullptr;
}

Shader* ResourceManager::GetShader(std::string _filePath)
{
	return shaderManager.Get(_filePath);
}

bool ResourceManager::UnloadShader(Shader* _shader)
{
	return shaderManager.Unload(_shader->filePath);
}

Texture* ResourceManager::LoadCubemap(const CubemapImportInfos& _filePaths, std::string _assetName, TextureFormat _format, bool _computeMipmap)
{
	std::filesystem::path filepath(_filePaths.right);
	filepath.remove_filename();
	filepath.append(_assetName);

	Texture* cubemap = GetCubemap(filepath.string());
	if (cubemap != nullptr)
		return cubemap;

	RawTexture rawCubemap = assetLoader.LoadCubemap(_filePaths, _format);
	if (rawCubemap.isValid)
	{
		rawCubemap.mipLevels = 1;
		if (_computeMipmap)
			rawCubemap.mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(rawCubemap.width, rawCubemap.height)))) + 1;

		Texture* newCubemap = new Texture();
		newCubemap->iTexture = CreateTexture(rawCubemap);
		newCubemap->filePath = filepath.string();
		newCubemap->height = rawCubemap.height;
		newCubemap->width = rawCubemap.width;
		newCubemap->mipLevels = rawCubemap.mipLevels;
		newCubemap->imageSize = rawCubemap.imageSize;
		newCubemap->imageCount = 6;
		newCubemap->isHDR = rawCubemap.isHdr;
		textureManager.Add(newCubemap->filePath, newCubemap);

		return newCubemap;
	}

	return nullptr;
}

Texture* ResourceManager::CubemapFromTexture(Texture* _texture, Mathlib::Vec2 _generatedTextureSize, bool _computeMipmap)
{
	std::filesystem::path filename(_texture->filePath);
	filename.replace_extension();
	filename.concat("Cubemap");

	Texture* cubemap = GetCubemap(filename.string());
	if (cubemap != nullptr)
		return cubemap;

	Texture* newCubemap = new Texture();

	Mesh* skyboxMesh = LoadMesh("Resources/Engine/Models/cube.obj");
	Shader* skyboxVertShader = LoadShader("Resources/Engine/Shaders/HLSL/FilterCubeVertex.hlsl", ShaderStage::VERTEX);
	Shader* skyboxFragShader = LoadShader("Resources/Engine/Shaders/HLSL/TextureToCubemapPixel.hlsl", ShaderStage::FRAGMENT);

	CreateCubemap(_texture->iTexture, _generatedTextureSize, _computeMipmap, newCubemap, skyboxMesh, skyboxVertShader, skyboxFragShader);
	newCubemap->filePath = filename.string();
	newCubemap->isHDR = true;

	textureManager.Add(newCubemap->filePath, newCubemap);

	return newCubemap;
}

Texture* ResourceManager::GetCubemap(std::string _filePath)
{
	return textureManager.Get(_filePath);
}

bool ResourceManager::UnloadCubemap(Texture* _cubemap)
{
	return textureManager.Unload(_cubemap->filePath);
}

void ResourceManager::CreateSkyboxFromTexture(Texture* _texture, Mathlib::Vec2 _generatedTextureSize, RenderEngine::SceneGraph::Skybox* _output)
{
	_output->cubemap = CubemapFromTexture(_texture, _generatedTextureSize, false);

	Mesh* skyboxMesh = LoadMesh("Resources/Engine/Models/cube.obj");
	Shader* skyboxVertShader = LoadShader("Resources/Engine/Shaders/HLSL/FilterCubeVertex.hlsl", ShaderStage::VERTEX);

	std::filesystem::path irradianceFilename(_texture->filePath);
	irradianceFilename.replace_extension();
	irradianceFilename.concat("Irradiance");

	Texture* irradianceCubemap = GetCubemap(irradianceFilename.string());
	if (irradianceCubemap != nullptr)
		_output->irradianceMap = irradianceCubemap;
	else
	{
		irradianceCubemap = new Texture();

		Shader* irradianceFragShader = LoadShader("Resources/Engine/Shaders/HLSL/IrradianceConvolutionPixel.hlsl", ShaderStage::FRAGMENT);

		CreateCubemap(_output->cubemap->iTexture, _generatedTextureSize, false, irradianceCubemap, skyboxMesh, skyboxVertShader, irradianceFragShader);
		irradianceCubemap->filePath = irradianceFilename.string();
		irradianceCubemap->isHDR = true;

		textureManager.Add(irradianceCubemap->filePath, irradianceCubemap);

		_output->irradianceMap = irradianceCubemap;
	}

	std::filesystem::path prefilteredFilename(_texture->filePath);
	prefilteredFilename.replace_extension();
	prefilteredFilename.concat("Prefiltered");

	Texture* prefilterCubemap = GetCubemap(prefilteredFilename.string());
	if (prefilterCubemap != nullptr)
		_output->prefilterMap = prefilterCubemap;
	else
	{
		prefilterCubemap = new Texture();

		Shader* prefilterFragShader = LoadShader("Resources/Engine/Shaders/HLSL/PrefilterEnvmapPixel.hlsl", ShaderStage::FRAGMENT);

		CreatePrefilteredCubemap(_output->cubemap->iTexture, _generatedTextureSize, prefilterCubemap, skyboxMesh, skyboxVertShader, prefilterFragShader);
		prefilterCubemap->filePath = prefilteredFilename.string();
		prefilterCubemap->isHDR = true;

		textureManager.Add(prefilterCubemap->filePath, prefilterCubemap);

		_output->prefilterMap = prefilterCubemap;
	}
}

void ResourceManager::SaveAsset(Texture* _texture)
{
	uint32_t textureSize = _texture->ComputeTotalSize(_texture);

	std::vector<char> data = GetTextureContent(_texture, textureSize * _texture->imageCount);

	std::filesystem::path filePath(_texture->filePath);
	if (filePath.has_extension())
		filePath.replace_extension();
	filePath.concat(".asset");

	std::ofstream ofs;
	ofs.open(filePath, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
	ofs.write(reinterpret_cast<const char*>(&_texture->width), sizeof(int));
	ofs.write(reinterpret_cast<const char*>(&_texture->height), sizeof(int));
	ofs.write(reinterpret_cast<const char*>(&textureSize), sizeof(uint32_t));
	ofs.write(reinterpret_cast<const char*>(&_texture->imageCount), sizeof(uint32_t));
	ofs.write(reinterpret_cast<const char*>(&_texture->mipLevels), sizeof(uint32_t));
	ofs.write(reinterpret_cast<const char*>(&_texture->isHDR), sizeof(bool));
	ofs.write(data.data(), data.size());

	ofs.close();
}

Texture* ResourceManager::LoadAsset(std::string _filePath)
{
	std::filesystem::path filename(_filePath);
	if (filename.extension() == ".asset")
	{
		std::ifstream file(_filePath, std::ios::in | std::ios::binary);
		if (!file.is_open())
			return { };

		RawTexture rawTexture;

		file.read(reinterpret_cast<char*>(&rawTexture.width), sizeof(int));
		file.read(reinterpret_cast<char*>(&rawTexture.height), sizeof(int));
		file.read(reinterpret_cast<char*>(&rawTexture.imageSize), sizeof(uint32_t));
		file.read(reinterpret_cast<char*>(&rawTexture.imageCount), sizeof(uint32_t));
		file.read(reinterpret_cast<char*>(&rawTexture.mipLevels), sizeof(uint32_t));
		file.read(reinterpret_cast<char*>(&rawTexture.isHdr), sizeof(bool));


		char* data = new char[rawTexture.imageCount * rawTexture.imageSize];
		file.read(data, rawTexture.imageCount * rawTexture.imageSize);
		rawTexture.data.insert(rawTexture.data.end(), &data[0], &data[rawTexture.imageCount * rawTexture.imageSize - 1]);

		file.close();


		Texture* newTexture = new Texture();
		newTexture->iTexture = CreateTexture(rawTexture, false);
		newTexture->filePath = _filePath;
		newTexture->height = rawTexture.height;
		newTexture->width = rawTexture.width;
		newTexture->imageSize = rawTexture.imageSize;
		newTexture->mipLevels = rawTexture.mipLevels;
		newTexture->imageCount = rawTexture.imageCount;
		newTexture->isHDR = rawTexture.isHdr;
		textureManager.Add(_filePath, newTexture);

		delete[]data;

		return newTexture;
	}

}

void ResourceManager::Clean()
{
	meshManager.Clean();
	textureManager.Clean();
	shaderManager.Clean();
}