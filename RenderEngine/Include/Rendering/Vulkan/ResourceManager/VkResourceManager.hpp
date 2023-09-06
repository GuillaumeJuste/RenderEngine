#pragma once

#ifndef RENDERENGINE_VKRESOURCEMANAGER
#define RENDERENGINE_VKRESOURCEMANAGER

#include "Rendering/Base/ResourceManager/ResourceManager.hpp"
#include "Rendering/Vulkan/Context/RenderContext/RenderContext.hpp"
#include "Rendering/Vulkan/ResourceManager/VkResourceManagerCreateInfo.hpp"

namespace RenderEngine::Rendering
{
	class VkResourceManager : public ResourceManager
	{
	private:
		/// Physical device
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		/// logical device
		VkDevice logicalDevice;

		/// graphics queue
		VkQueue graphicsQueue;

		CommandPool* commandPool;

		bool CreateMesh(const Loader::RawMesh& _input, RenderEngine::Assets::Mesh* _output);
		void CreateVertexBufferObject(const Loader::RawMesh& _input, RenderEngine::Assets::Mesh* _output);
		void CreateIndexBufferObject(const Loader::RawMesh& _input, RenderEngine::Assets::Mesh* _output);

		bool CreateTexture(const Loader::RawTexture& _input, RenderEngine::Assets::Texture* _output, bool _generateMipMap = true);
		bool CreateShader(const Loader::RawShader& _input, RenderEngine::Assets::Shader* _output);
		bool CreateCubemap(ITexture* _texture, Mathlib::Vec2 _outputSize, bool _generateMipmap, RenderEngine::Assets::Texture* _output,
			RenderEngine::Assets::Mesh* _mesh, RenderEngine::Assets::Shader* _vertexShader, RenderEngine::Assets::Shader* _fragmentShader);
		bool CreatePrefilteredCubemap(ITexture* _texture, Mathlib::Vec2 _outputSize, RenderEngine::Assets::Texture* _output,
			RenderEngine::Assets::Mesh* _mesh, RenderEngine::Assets::Shader* _vertexShader, RenderEngine::Assets::Shader* _fragmentShader);

		std::vector<char> GetTextureContent(RenderEngine::Assets::Texture* _texture, uint32_t _imageTotalSize);
	public:
		VkResourceManager(VkResourceManagerCreateInfo _createInfo);
	};

}

#endif