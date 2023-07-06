#include "Rendering/Vulkan/Context/RenderContext/RenderContext.hpp"

#include <iostream>
#include <array>
#include "ResourceManager/Assets/Mesh/Vertex.hpp"
#include "Rendering/Vulkan/Shader/VkShaderCreateInfo.hpp"
#include "Rendering/Vulkan/Shader/VkShader.hpp"
#include "Rendering/Vulkan/Misc/Utils.hpp"

using namespace RenderEngine::Rendering;
using namespace RenderEngine::Assets;

void RenderContext::InitalizeRenderContext(const RenderContextVkCreateInfo& _createInfo, RenderContext* _output)
{
	_output->instance = _createInfo.instance;
	_output->windowProperties = _createInfo.windowProperties;
	_output->physicalDeviceProperties = _createInfo.physicalDeviceProperties;
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->queueFamilyIndices = _createInfo.physicalDeviceProperties.queueFamilyIndices;
	_output->graphicsQueue = _createInfo.graphicsQueue;
	_output->presentQueue = _createInfo.presentQueue;
	_output->commandPool = _createInfo.commandPool;

	_output->windowProperties->window->FramebufferResizeEvent.Add(_output, &RenderContext::FrameBufferResizedCallback);

	_output->CreateSwapChain();
	_output->createColorResources();
	_output->CreateDepthBuffer();
	_output->CreateRenderPass();
	_output->CreateFrameBuffer();
	_output->CreateCommandBuffer(*_createInfo.renderContextCreateInfo.swapChainCommandBufferCreateInfo);

}

void RenderContext::CreateSwapChain()
{
	SwapChainVkCreateInfo createInfo;
	createInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	createInfo.logicalDevice = logicalDevice;
	createInfo.windowProperties = windowProperties;
	createInfo.queueFamilyIndices = queueFamilyIndices;

	SwapChain::InitializeSwapChain(createInfo, &swapChain);
}

void RenderContext::CreateRenderPass()
{
	RenderPassVkCreateInfo createInfo{};
	createInfo.logicalDevice = logicalDevice;
	createInfo.swapChainImageFormat = swapChain.GetImageFormat();
	createInfo.depthBuffer = &depthBuffer;
	createInfo.samples = physicalDeviceProperties.msaaSamples;
	createInfo.createColorResolveAttachement = true;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcAccessMask = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	createInfo.subpassDependencies.push_back(dependency);

	RenderPass::InitializeRenderPass(createInfo, &renderPass);
}

void RenderContext::CreateFrameBuffer()
{
	FrameBufferVkCreateInfo createInfo{};
	createInfo.logicalDevice = logicalDevice;
	createInfo.renderPass = &renderPass;
	createInfo.imageViews = swapChain.GetImageViews();
	createInfo.swapChainImageCount = swapChain.GetImageCount();
	createInfo.swapChainExtent = swapChain.GetExtent();
	createInfo.depthBuffer = &depthBuffer;
	createInfo.colorImage = &colorImage;

	FrameBuffer::InitializeFrameBuffer(createInfo, &frameBuffer);
}

void RenderContext::createColorResources()
{
	VkFormat colorFormat = swapChain.GetImageFormat();

	ImageVkCreateInfo imageCreateInfo{};
	imageCreateInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	imageCreateInfo.logicalDevice = logicalDevice;
	imageCreateInfo.width = static_cast<uint32_t>(swapChain.GetExtent().width);
	imageCreateInfo.height = static_cast<uint32_t>(swapChain.GetExtent().height);
	imageCreateInfo.format = colorFormat;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	imageCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	imageCreateInfo.commandPool = commandPool;
	imageCreateInfo.graphicsQueue = graphicsQueue;
	imageCreateInfo.textureCount = 1;
	imageCreateInfo.imageFlags = 0;
	imageCreateInfo.imageViewType = VK_IMAGE_VIEW_TYPE_2D;
	imageCreateInfo.imageViewAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.samples = physicalDeviceProperties.msaaSamples;
	Image::InitializeImage(imageCreateInfo, &colorImage);
}

void RenderContext::CreateDepthBuffer()
{
	DepthBufferVkCreateInfo createInfo{};
	createInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	createInfo.logicalDevice = logicalDevice;
	createInfo.commandPool = commandPool;
	createInfo.graphicsQueue = graphicsQueue;
	createInfo.swapChainExtent = swapChain.GetExtent();
	createInfo.samples = physicalDeviceProperties.msaaSamples;
	
	DepthBuffer::InitializeDepthBuffer(createInfo, &depthBuffer);
}

void RenderContext::CreateCommandBuffer(const SwapChainCommandBufferCreateInfo& _createInfo)
{
	SwapChainCommandBufferVkCreateInfo createInfo{};
	createInfo.commandBufferCreateInfo = _createInfo;
	createInfo.logicalDevice = logicalDevice;
	createInfo.commandPool = commandPool;
	createInfo.renderPass = &renderPass;
	createInfo.frameBuffer = &frameBuffer;
	createInfo.swapChain = &swapChain;

	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		SwapChainCommandBuffer::InitializeCommandBuffer(createInfo, &commandBuffers[i]);
	}
}

void RenderContext::CleanUpSwapChain()
{
	depthBuffer.Cleanup();
	colorImage.Cleanup();
	frameBuffer.Cleanup();
	swapChain.Cleanup();
}

void RenderContext::RecreateSwapChain()
{
	int width = 0, height = 0;
	windowProperties->window->GetFrameBufferSize(&width, &height);
	while (width == 0 || height == 0) {
		windowProperties->window->GetFrameBufferSize(&width, &height);
	}

	vkDeviceWaitIdle(logicalDevice);

	CleanUpSwapChain();

	CreateSwapChain();

	createColorResources();

	CreateDepthBuffer();

	CreateFrameBuffer();

	frameBufferWasResized = false;
}

void RenderContext::FrameBufferResizedCallback()
{
	frameBufferWasResized = true;
}

void RenderContext::DrawScene(RenderEngine::SceneGraph::Scene* _scene)
{
	VkScene* vkScene = WasSceneLoaded(_scene);

	if (vkScene == nullptr)
	{
		vkScene = LoadScene(_scene);
	}

	vkScene->Update(currentFrame);

	vkWaitForFences(logicalDevice, 1, &commandBuffers[currentFrame].GetInFlightFence(), VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain.GetVKSwapChain(), UINT64_MAX, commandBuffers[currentFrame].GetImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetFences(logicalDevice, 1, &commandBuffers[currentFrame].GetInFlightFence());

	vkResetCommandBuffer(commandBuffers[currentFrame].GetVKCommandBuffer(), 0);

	commandBuffers[currentFrame].RecordCommandBuffer(imageIndex, currentFrame, vkScene);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { commandBuffers[currentFrame].GetImageAvailableSemaphore() };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame].GetVKCommandBuffer();

	VkSemaphore signalSemaphores[] = { commandBuffers[currentFrame].GetRenderFinishedSemaphore() };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, commandBuffers[currentFrame].GetInFlightFence()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain.GetVKSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || frameBufferWasResized)
	{
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

VkScene* RenderContext::LoadScene(RenderEngine::SceneGraph::Scene* _scene)
{
	SceneData* data = &scenesData.emplace_front();
	data->scene = _scene;

	VkSceneCreateInfo createInfo{};
	createInfo.commandPool = commandPool;
	createInfo.graphicsQueue = graphicsQueue;
	createInfo.logicalDevice = logicalDevice;
	createInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	createInfo.samples = physicalDeviceProperties.msaaSamples;
	createInfo.renderpass = &renderPass;
	createInfo.swapchain = &swapChain;
	createInfo.scene = _scene;

	data->vkScene = VkScene(createInfo);

	return &data->vkScene;
}

VkScene* RenderContext::WasSceneLoaded(RenderEngine::SceneGraph::Scene* _scene)
{
	for (std::forward_list<SceneData>::iterator it = scenesData.begin(); it != scenesData.end(); ++it)
	{
		if ((*it).scene == _scene)
		{
			return &(*it).vkScene;
		}
	}
	return nullptr;
}

bool RenderContext::CreateMesh(const RenderEngine::Assets::RawMesh& _input, RenderEngine::Assets::Mesh* _output)
{
	CreateVertexBufferObject(_input, _output);
	CreateIndexBufferObject(_input, _output);
	return true;
}

void RenderContext::CreateVertexBufferObject(const RenderEngine::Assets::RawMesh& _input, RenderEngine::Assets::Mesh* _output)
{
	BufferObject stagingBufferObject;
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	stagingBufferCreateInfo.logicalDevice = logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = sizeof(_input.vertices[0]) * _input.vertices.size();

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBufferObject);

	void* data;
	vkMapMemory(logicalDevice, stagingBufferObject.GetVkBufferMemory(), 0, stagingBufferCreateInfo.bufferSize, 0, &data);
	memcpy(data, _input.vertices.data(), (size_t)stagingBufferCreateInfo.bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferObject.GetVkBufferMemory());

	BufferObjectVkCreateInfo vertexBuffeCreateInfo;
	vertexBuffeCreateInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	vertexBuffeCreateInfo.logicalDevice = logicalDevice;
	vertexBuffeCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	vertexBuffeCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	vertexBuffeCreateInfo.bufferSize = stagingBufferCreateInfo.bufferSize;

	BufferObject* VBO = new BufferObject();

	BufferObject::InitializeBufferObject(vertexBuffeCreateInfo, VBO);

	stagingBufferObject.CopyBuffer(VBO, commandPool, graphicsQueue, stagingBufferCreateInfo.bufferSize);
	stagingBufferObject.Clean();

	_output->vertexBuffer = VBO;
}

void RenderContext::CreateIndexBufferObject(const RenderEngine::Assets::RawMesh& _input, RenderEngine::Assets::Mesh* _output)
{
	BufferObject stagingBufferObject;
	BufferObjectVkCreateInfo stagingBufferCreateInfo;
	stagingBufferCreateInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	stagingBufferCreateInfo.logicalDevice = logicalDevice;
	stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	stagingBufferCreateInfo.bufferSize = sizeof(_input.indices[0]) * _input.indices.size();

	BufferObject::InitializeBufferObject(stagingBufferCreateInfo, &stagingBufferObject);

	void* data;
	vkMapMemory(logicalDevice, stagingBufferObject.GetVkBufferMemory(), 0, stagingBufferCreateInfo.bufferSize, 0, &data);
	memcpy(data, _input.indices.data(), (size_t)stagingBufferCreateInfo.bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferObject.GetVkBufferMemory());

	BufferObjectVkCreateInfo indexBufferCreateInfo;
	indexBufferCreateInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	indexBufferCreateInfo.logicalDevice = logicalDevice;
	indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	indexBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	indexBufferCreateInfo.bufferSize = stagingBufferCreateInfo.bufferSize;

	BufferObject* IBO = new BufferObject();

	BufferObject::InitializeBufferObject(indexBufferCreateInfo, IBO);

	stagingBufferObject.CopyBuffer(IBO, commandPool, graphicsQueue, stagingBufferCreateInfo.bufferSize);
	stagingBufferObject.Clean();

	_output->indexBuffer = IBO;
}

bool RenderContext::CreateTexture(const RenderEngine::Assets::RawTexture& _input, RenderEngine::Assets::Texture* _output)
{
	VkTextureVkCreateInfo textCreateInfo{};
	textCreateInfo.logicalDevice = logicalDevice;
	textCreateInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	textCreateInfo.graphicsQueue = graphicsQueue;
	textCreateInfo.commandPool = commandPool;
	textCreateInfo.texture = _input;
	textCreateInfo.width = _input.width;
	textCreateInfo.height = _input.height;
	textCreateInfo.mipLevels = _input.mipLevels;
	textCreateInfo.imageCount = _input.imageCount;
	textCreateInfo.format = _input.isHdr ? VK_FORMAT_R32G32B32A32_SFLOAT : VK_FORMAT_R8G8B8A8_SRGB;
	textCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	textCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	textCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	textCreateInfo.imageFlags = 0;
	textCreateInfo.imageViewType = VK_IMAGE_VIEW_TYPE_2D;

	VkTexture* vkTexture = new VkTexture();

	VkTexture::InitializeVkTexture(textCreateInfo, vkTexture);

	_output->iTexture = vkTexture;
	return true;
}

bool RenderContext::CreateShader(const RenderEngine::Assets::RawShader& _input, RenderEngine::Assets::Shader* _output)
{
	VkShaderCreateInfo shaderCreateInfo{};
	shaderCreateInfo.rawShader = _input;
	shaderCreateInfo.device = logicalDevice;

	VkShader* vkShader = new VkShader();

	VkShader::CreateVkShader(shaderCreateInfo, vkShader);
	_output->iShader = vkShader;

	return true;
}

bool RenderContext::CreateCubemap(const RenderEngine::Assets::RawTexture& _input, RenderEngine::Assets::Cubemap* _output)
{
	VkTextureVkCreateInfo textCreateInfo{};
	textCreateInfo.logicalDevice = logicalDevice;
	textCreateInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	textCreateInfo.graphicsQueue = graphicsQueue;
	textCreateInfo.commandPool = commandPool;
	textCreateInfo.texture = _input;
	textCreateInfo.width = _input.width;
	textCreateInfo.height = _input.height;
	textCreateInfo.mipLevels = _input.mipLevels;
	textCreateInfo.imageCount = _input.imageCount;
	textCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	textCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	textCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	textCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	textCreateInfo.imageFlags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	textCreateInfo.imageViewType = VK_IMAGE_VIEW_TYPE_CUBE;

	VkTexture* vkTexture = new VkTexture();

	VkTexture::InitializeVkTexture(textCreateInfo, vkTexture);

	_output->iTexture = vkTexture;
	return true;
}

bool RenderContext::CreateCubemap(ITexture* _texture, Mathlib::Vec2 _outputSize, bool _generateMipmap, RenderEngine::Assets::Cubemap* _output,
	RenderEngine::Assets::Mesh* _mesh, RenderEngine::Assets::Shader* _vertexShader, RenderEngine::Assets::Shader* _fragmentShader)
{
	const VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;
	const int32_t width = (int32_t)_outputSize.X;
	const int32_t height = (int32_t)_outputSize.Y;
	uint32_t numMips = 1;
	if(_generateMipmap)
		numMips = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;

	_output->width = width;
	_output->height = height;
	_output->mipLevels = numMips;
	_output->imageSize = width * height * 16 /* 4 channels * sizeof(float) */;

	// output texture

	VkTextureVkCreateInfo textCreateInfo{};
	textCreateInfo.logicalDevice = logicalDevice;
	textCreateInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	textCreateInfo.graphicsQueue = graphicsQueue;
	textCreateInfo.commandPool = commandPool;
	textCreateInfo.width = width;
	textCreateInfo.height = height;
	textCreateInfo.mipLevels = numMips;
	textCreateInfo.imageCount = 6;
	textCreateInfo.format = format;
	textCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	textCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	textCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	textCreateInfo.imageFlags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	textCreateInfo.imageViewType = VK_IMAGE_VIEW_TYPE_CUBE;

	VkTexture* outputTexture = new VkTexture();

	VkTexture::InitializeVkTexture(textCreateInfo, outputTexture, false);
	outputTexture->GetImage()->TransitionImageLayout(format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	// initialize renderpass

	VkSubpassDependency firstDependency{};
	firstDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	firstDependency.dstSubpass = 0;
	firstDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	firstDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	firstDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	firstDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	firstDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkSubpassDependency secondDependency{};
	secondDependency.srcSubpass = 0;
	secondDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
	secondDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	secondDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	secondDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	secondDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	secondDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	RenderPassVkCreateInfo renderPassCreateInfo{};
	renderPassCreateInfo.logicalDevice = logicalDevice;
	renderPassCreateInfo.swapChainImageFormat = format;
	renderPassCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	renderPassCreateInfo.createColorResolveAttachement = false;
	renderPassCreateInfo.subpassDependencies.push_back(firstDependency);
	renderPassCreateInfo.subpassDependencies.push_back(secondDependency);

	RenderPass tmpRenderPass;

	RenderPass::InitializeRenderPass(renderPassCreateInfo, &tmpRenderPass);

	struct {
		Image image;
		FrameBuffer framebuffer;
	} offscreen;

	// Offscreen framebuffer
	{
		ImageVkCreateInfo imageCreateInfo{};
		imageCreateInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
		imageCreateInfo.logicalDevice = logicalDevice;
		imageCreateInfo.width = width;
		imageCreateInfo.height = height;
		imageCreateInfo.format = format;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		imageCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		imageCreateInfo.commandPool = commandPool;
		imageCreateInfo.graphicsQueue = graphicsQueue;
		imageCreateInfo.textureCount = 1;
		imageCreateInfo.imageFlags = 0;
		imageCreateInfo.imageViewType = VK_IMAGE_VIEW_TYPE_2D;
		imageCreateInfo.imageViewAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCreateInfo.mipLevels = 1;
		Image::InitializeImage(imageCreateInfo, &offscreen.image);

		FrameBufferVkCreateInfo createInfo{};
		createInfo.logicalDevice = logicalDevice;
		createInfo.renderPass = &tmpRenderPass;
		createInfo.imageViews.push_back(offscreen.image.GetImageView());
		createInfo.swapChainImageCount = 1;
		createInfo.swapChainExtent = VkExtent2D{ (unsigned int)width, (unsigned int)height};
		createInfo.depthBuffer = nullptr;
		createInfo.colorImage = nullptr;

		FrameBuffer::InitializeFrameBuffer(createInfo, &offscreen.framebuffer);

		offscreen.image.TransitionImageLayout(format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	}

	// Pipeline layout
	struct defaultCameraBlock {
		Mathlib::Mat4 invView;
		Mathlib::Mat4 proj;
		Mathlib::Vec3 cameraPos;
	} cameraBlock;

	VkTexture* inputTexture = dynamic_cast<VkTexture*>(_texture);

	DescriptorDataList fShaderDatalist{};
	DescriptorData textureBufferData{};
	textureBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	textureBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	textureBufferData.binding = 0;
	textureBufferData.texture = inputTexture;
	fShaderDatalist.Add(textureBufferData);

	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(defaultCameraBlock);

	GraphicsPipelineVkCreateInfo gpCreateInfo{};
	gpCreateInfo.logicalDevice = logicalDevice;
	gpCreateInfo.renderPass = &tmpRenderPass;
	gpCreateInfo.swapChainExtent = VkExtent2D{ (unsigned int)width, (unsigned int)height };
	gpCreateInfo.swapChainImageFormat = format;
	gpCreateInfo.vertexShader = _vertexShader;
	gpCreateInfo.fragmentShader = _fragmentShader;
	gpCreateInfo.drawMode = PolygonDrawMode::FILL;
	gpCreateInfo.lineWidth = 1.0f;
	gpCreateInfo.frontFace = FrontFace::COUNTER_CLOCKWISE;
	gpCreateInfo.enableDepthTest = VK_FALSE;
	gpCreateInfo.writeDepthBuffer = VK_FALSE;
	gpCreateInfo.culling_mode = VK_CULL_MODE_FRONT_BIT;
	gpCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;

	gpCreateInfo.descriptorDatas.push_back(fShaderDatalist);
	gpCreateInfo.pushConstants.push_back(pushConstantRange);

	GraphicsPipeline tmpPipeline;

	GraphicsPipeline::InitalizeGraphicsPipeline(gpCreateInfo, &tmpPipeline);

	std::vector<DescriptorSet> descriptorSets;

	size_t descrtiptorSetCount = gpCreateInfo.descriptorDatas.size();
	descriptorSets.resize(descrtiptorSetCount);

	for (int index = 0; index < descrtiptorSetCount; index++)
	{
		DescriptorSetVkCreateInfo descriptorSetCreateInfo{};
		descriptorSetCreateInfo.logicalDevice = logicalDevice;
		descriptorSetCreateInfo.descriptorSetLayout = tmpPipeline.GetDescriptorSetLayout(index);
		descriptorSetCreateInfo.descriptorPool = tmpPipeline.GetDescriptorPool(index);
		descriptorSetCreateInfo.descriptorDatas = gpCreateInfo.descriptorDatas[index];
		descriptorSetCreateInfo.frameCount = 1;

		DescriptorSet::InitializeDescriptorSet(descriptorSetCreateInfo, &descriptorSets[index]);
	}

	// Render

	VkClearValue clearValues[1]{};
	clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };

	VkRenderPassBeginInfo renderPassBeginInfo{};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	// Reuse render pass from example pass
	renderPassBeginInfo.renderPass = tmpRenderPass.GetRenderPass();
	renderPassBeginInfo.framebuffer = offscreen.framebuffer.GetFrameBuffers()[0];
	renderPassBeginInfo.renderArea.extent.width = width;
	renderPassBeginInfo.renderArea.extent.height = height;
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = clearValues;
	renderPassBeginInfo.pNext = nullptr;
	renderPassBeginInfo.renderArea.offset = { 0, 0 };

	std::vector<Mathlib::Mat4> matrices = {
		// POSITIVE_X
		Mathlib::Mat4::InvViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.f, 0.f, 0.f), Mathlib::Vec3(1.f, 0.f, 0.f), Mathlib::Vec3(0.f, 1.f, 0.f)).Transpose(),
		// NEGATIVE_X
		Mathlib::Mat4::InvViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.f, 0.f, 0.f), Mathlib::Vec3(-1.f, 0.f, 0.f), Mathlib::Vec3(0.f, 1.f, 0.f)).Transpose(),
		// POSITIVE_Y
		Mathlib::Mat4::InvViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.f, 0.f, 0.f), Mathlib::Vec3(0.f, -1.f, 0.f), Mathlib::Vec3(0.f, 0.f, -1.f)).Transpose(),
		// NEGATIVE_Y
		Mathlib::Mat4::InvViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.f, 0.f, 0.f), Mathlib::Vec3(0.f, 1.f, 0.f), Mathlib::Vec3(0.f, 0.f, 1.f)).Transpose(),
		// POSITIVE_Z
		Mathlib::Mat4::InvViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.f, 0.f, 0.f), Mathlib::Vec3(0.f, 0.f, 1.f), Mathlib::Vec3(0.f, 1.f, 0.f)).Transpose(),
		// NEGATIVE_Z
		Mathlib::Mat4::InvViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.f, 0.f, 0.f), Mathlib::Vec3(0.f, 0.f, -1.f), Mathlib::Vec3(0.f, 1.f, 0.f)).Transpose(),
	};

	Mathlib::Mat4 projection = Mathlib::Mat4::PerspectiveMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Math::Radians(90), 1.0f, 0.1f, 512.0f).Transpose();

	VkViewport viewport{};
	viewport.width = _outputSize.X;
	viewport.height = -_outputSize.Y;
	viewport.x = 0.f;
	viewport.y = _outputSize.Y;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;

	VkRect2D scissor{};
	scissor.extent.width = width;
	scissor.extent.height = height;
	scissor.offset.x = 0;
	scissor.offset.y = 0;

	VkCommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(logicalDevice, commandPool);
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	BufferObject*  VBO = dynamic_cast<BufferObject*>(_mesh->vertexBuffer);
	BufferObject*  IBO = dynamic_cast<BufferObject*>(_mesh->indexBuffer);
	for (uint32_t m = 0; m < numMips; m++)
	{
		viewport.width = static_cast<float>(width * std::pow(0.5f, m));
		viewport.height = static_cast<float>(-height * std::pow(0.5f, m));
		viewport.y = -viewport.height;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		for (uint32_t f = 0; f < 6; f++)
		{
			// Render scene from cube face's point of view
			vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

			// Update shader push constant block
			cameraBlock.invView = matrices[f];
			cameraBlock.proj = projection;
			cameraBlock.cameraPos = Mathlib::Vec3(0.f, 0.f, 0.f);

			vkCmdPushConstants(commandBuffer, tmpPipeline.GetGraphicsPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(defaultCameraBlock), &cameraBlock);

			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, tmpPipeline.GetGraphicsPipeline());

			size_t descrtiptorSetCount = descriptorSets.size();

			for (int index = 0; index < descrtiptorSetCount; index++)
			{
				vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, tmpPipeline.GetGraphicsPipelineLayout(), index, 1, &descriptorSets[index].GetFrameDescriptorSet(0), 0, nullptr);
			}

			VkBuffer vertexBuffers[] = { VBO->GetVkBuffer() };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(commandBuffer, IBO->GetVkBuffer(), 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(_mesh->indiceCount), 1, 0, 0, 0);

			vkCmdEndRenderPass(commandBuffer);

			offscreen.image.TransitionImageLayout(format, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, commandBuffer);

			// Copy region for transfer from framebuffer to cube face
			VkImageCopy copyRegion = {};

			copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			copyRegion.srcSubresource.baseArrayLayer = 0;
			copyRegion.srcSubresource.mipLevel = 0;
			copyRegion.srcSubresource.layerCount = 1;
			copyRegion.srcOffset = { 0, 0, 0 };

			copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			copyRegion.dstSubresource.baseArrayLayer = f;
			copyRegion.dstSubresource.mipLevel = m;
			copyRegion.dstSubresource.layerCount = 1;
			copyRegion.dstOffset = { 0, 0, 0 };

			copyRegion.extent.width = static_cast<uint32_t>(viewport.width);
			copyRegion.extent.height = -static_cast<uint32_t>(viewport.height);
			copyRegion.extent.depth = 1;

			vkCmdCopyImage(
				commandBuffer,
				offscreen.image.GetVkImage(),
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				outputTexture->GetImage()->GetVkImage(),
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&copyRegion);

			offscreen.image.TransitionImageLayout(format, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, commandBuffer);

		}
	}
	outputTexture->GetImage()->TransitionImageLayout(format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, commandBuffer);

	CommandBuffer::EndSingleTimeCommands(logicalDevice, commandPool, graphicsQueue, commandBuffer);

	tmpPipeline.Cleanup();
	offscreen.framebuffer.Cleanup();
	offscreen.image.Cleanup();
	tmpRenderPass.Cleanup();

	_output->iTexture = outputTexture;

	return true;
}

bool RenderContext::CreatePrefilteredCubemap(ITexture* _texture, Mathlib::Vec2 _outputSize, RenderEngine::Assets::Cubemap* _output,
	RenderEngine::Assets::Mesh* _mesh, RenderEngine::Assets::Shader* _vertexShader, RenderEngine::Assets::Shader* _fragmentShader)
{
	const VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;
	const int32_t width = (int32_t)_outputSize.X;
	const int32_t height = (int32_t)_outputSize.Y;
	const uint32_t numMips = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;


	_output->width = width;
	_output->height = height;
	_output->mipLevels = numMips;
	_output->imageSize = width * height * 16 /* 4 channels * sizeof(float) */;

	// output texture

	VkTextureVkCreateInfo textCreateInfo{};
	textCreateInfo.logicalDevice = logicalDevice;
	textCreateInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
	textCreateInfo.graphicsQueue = graphicsQueue;
	textCreateInfo.commandPool = commandPool;
	textCreateInfo.width = width;
	textCreateInfo.height = height;
	textCreateInfo.mipLevels = numMips;
	textCreateInfo.imageCount = 6;
	textCreateInfo.format = format;
	textCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	textCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	textCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	textCreateInfo.imageFlags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	textCreateInfo.imageViewType = VK_IMAGE_VIEW_TYPE_CUBE;

	VkTexture* outputTexture = new VkTexture();

	VkTexture::InitializeVkTexture(textCreateInfo, outputTexture, false);
	outputTexture->GetImage()->TransitionImageLayout(format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	// initialize renderpass

	VkSubpassDependency firstDependency{};
	firstDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	firstDependency.dstSubpass = 0;
	firstDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	firstDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	firstDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	firstDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	firstDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkSubpassDependency secondDependency{};
	secondDependency.srcSubpass = 0;
	secondDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
	secondDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	secondDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	secondDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	secondDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	secondDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	RenderPassVkCreateInfo renderPassCreateInfo{};
	renderPassCreateInfo.logicalDevice = logicalDevice;
	renderPassCreateInfo.swapChainImageFormat = format;
	renderPassCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	renderPassCreateInfo.createColorResolveAttachement = false;
	renderPassCreateInfo.subpassDependencies.push_back(firstDependency);
	renderPassCreateInfo.subpassDependencies.push_back(secondDependency);

	RenderPass tmpRenderPass;

	RenderPass::InitializeRenderPass(renderPassCreateInfo, &tmpRenderPass);

	struct {
		Image image;
		FrameBuffer framebuffer;
	} offscreen;

	// Offscreen framebuffer
	{
		ImageVkCreateInfo imageCreateInfo{};
		imageCreateInfo.physicalDevice = physicalDeviceProperties.physicalDevice;
		imageCreateInfo.logicalDevice = logicalDevice;
		imageCreateInfo.width = width;
		imageCreateInfo.height = height;
		imageCreateInfo.format = format;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		imageCreateInfo.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		imageCreateInfo.commandPool = commandPool;
		imageCreateInfo.graphicsQueue = graphicsQueue;
		imageCreateInfo.textureCount = 1;
		imageCreateInfo.imageFlags = 0;
		imageCreateInfo.imageViewType = VK_IMAGE_VIEW_TYPE_2D;
		imageCreateInfo.imageViewAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCreateInfo.mipLevels = 1;
		Image::InitializeImage(imageCreateInfo, &offscreen.image);

		FrameBufferVkCreateInfo createInfo{};
		createInfo.logicalDevice = logicalDevice;
		createInfo.renderPass = &tmpRenderPass;
		createInfo.imageViews.push_back(offscreen.image.GetImageView());
		createInfo.swapChainImageCount = 1;
		createInfo.swapChainExtent = VkExtent2D{ (unsigned int)width, (unsigned int)height };
		createInfo.depthBuffer = nullptr;
		createInfo.colorImage = nullptr;

		FrameBuffer::InitializeFrameBuffer(createInfo, &offscreen.framebuffer);

		offscreen.image.TransitionImageLayout(format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	}

	// Pipeline layout

	struct PushBlock {
		Mathlib::Mat4 invView;
		Mathlib::Mat4 proj;
		float roughness = 0.f;
		uint32_t numSamples = 32u;
	} pushBlock;

	VkPushConstantRange cameraPushConstantRange{};
	cameraPushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	cameraPushConstantRange.offset = 0;
	cameraPushConstantRange.size = sizeof(PushBlock);

	//fragment shader descriptor set
	VkTexture* inputTexture = dynamic_cast<VkTexture*>(_texture);

	DescriptorDataList fShaderDatalist{};
	DescriptorData textureBufferData{};
	textureBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	textureBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	textureBufferData.binding = 0;
	textureBufferData.texture = inputTexture;
	fShaderDatalist.Add(textureBufferData);

	GraphicsPipelineVkCreateInfo gpCreateInfo{};
	gpCreateInfo.logicalDevice = logicalDevice;
	gpCreateInfo.renderPass = &tmpRenderPass;
	gpCreateInfo.swapChainExtent = VkExtent2D{ (unsigned int)width, (unsigned int)height };
	gpCreateInfo.swapChainImageFormat = format;
	gpCreateInfo.vertexShader = _vertexShader;
	gpCreateInfo.fragmentShader = _fragmentShader;
	gpCreateInfo.drawMode = PolygonDrawMode::FILL;
	gpCreateInfo.lineWidth = 1.0f;
	gpCreateInfo.frontFace = FrontFace::COUNTER_CLOCKWISE;
	gpCreateInfo.enableDepthTest = VK_FALSE;
	gpCreateInfo.writeDepthBuffer = VK_FALSE;
	gpCreateInfo.culling_mode = VK_CULL_MODE_FRONT_BIT;
	gpCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;

	gpCreateInfo.descriptorDatas.push_back(fShaderDatalist);
	gpCreateInfo.pushConstants.push_back(cameraPushConstantRange);

	GraphicsPipeline tmpPipeline;

	GraphicsPipeline::InitalizeGraphicsPipeline(gpCreateInfo, &tmpPipeline);

	std::vector<DescriptorSet> descriptorSets;

	size_t descrtiptorSetCount = gpCreateInfo.descriptorDatas.size();
	descriptorSets.resize(descrtiptorSetCount);

	for (int index = 0; index < descrtiptorSetCount; index++)
	{
		DescriptorSetVkCreateInfo descriptorSetCreateInfo{};
		descriptorSetCreateInfo.logicalDevice = logicalDevice;
		descriptorSetCreateInfo.descriptorSetLayout = tmpPipeline.GetDescriptorSetLayout(index);
		descriptorSetCreateInfo.descriptorPool = tmpPipeline.GetDescriptorPool(index);
		descriptorSetCreateInfo.descriptorDatas = gpCreateInfo.descriptorDatas[index];
		descriptorSetCreateInfo.frameCount = 1;

		DescriptorSet::InitializeDescriptorSet(descriptorSetCreateInfo, &descriptorSets[index]);
	}

	// Render

	VkClearValue clearValues[1]{};
	clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };

	VkRenderPassBeginInfo renderPassBeginInfo{};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	// Reuse render pass from example pass
	renderPassBeginInfo.renderPass = tmpRenderPass.GetRenderPass();
	renderPassBeginInfo.framebuffer = offscreen.framebuffer.GetFrameBuffers()[0];
	renderPassBeginInfo.renderArea.extent.width = width;
	renderPassBeginInfo.renderArea.extent.height = height;
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = clearValues;
	renderPassBeginInfo.pNext = nullptr;
	renderPassBeginInfo.renderArea.offset = { 0, 0 };

	std::vector<Mathlib::Mat4> matrices = {
		// POSITIVE_X
		Mathlib::Mat4::InvViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.f, 0.f, 0.f), Mathlib::Vec3(1.f, 0.f, 0.f), Mathlib::Vec3(0.f, 1.f, 0.f)).Transpose(),
		// NEGATIVE_X
		Mathlib::Mat4::InvViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.f, 0.f, 0.f), Mathlib::Vec3(-1.f, 0.f, 0.f), Mathlib::Vec3(0.f, 1.f, 0.f)).Transpose(),
		// POSITIVE_Y
		Mathlib::Mat4::InvViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.f, 0.f, 0.f), Mathlib::Vec3(0.f, -1.f, 0.f), Mathlib::Vec3(0.f, 0.f, -1.f)).Transpose(),
		// NEGATIVE_Y
		Mathlib::Mat4::InvViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.f, 0.f, 0.f), Mathlib::Vec3(0.f, 1.f, 0.f), Mathlib::Vec3(0.f, 0.f, 1.f)).Transpose(),
		// POSITIVE_Z
		Mathlib::Mat4::InvViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.f, 0.f, 0.f), Mathlib::Vec3(0.f, 0.f, 1.f), Mathlib::Vec3(0.f, 1.f, 0.f)).Transpose(),
		// NEGATIVE_Z
		Mathlib::Mat4::InvViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Vec3(0.f, 0.f, 0.f), Mathlib::Vec3(0.f, 0.f, -1.f), Mathlib::Vec3(0.f, 1.f, 0.f)).Transpose(),
	};

	Mathlib::Mat4 projection = Mathlib::Mat4::PerspectiveMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Math::Radians(90), 1.0f, 0.1f, 512.0f).Transpose();

	VkViewport viewport{};
	viewport.width = _outputSize.X;
	viewport.height = -_outputSize.Y;
	viewport.x = 0.f;
	viewport.y = _outputSize.Y;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;

	VkRect2D scissor{};
	scissor.extent.width = width;
	scissor.extent.height = height;
	scissor.offset.x = 0;
	scissor.offset.y = 0;

	VkCommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(logicalDevice, commandPool);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	BufferObject* VBO = dynamic_cast<BufferObject*>(_mesh->vertexBuffer);
	BufferObject* IBO = dynamic_cast<BufferObject*>(_mesh->indexBuffer);
	for (uint32_t m = 0; m < numMips; m++)
	{
		pushBlock.roughness = (float)m / (float)(numMips - 1);

		viewport.width = static_cast<float>(width * std::pow(0.5f, m));
		viewport.height = static_cast<float>(-height * std::pow(0.5f, m));
		viewport.y = -viewport.height;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		for (uint32_t f = 0; f < 6; f++)
		{
			// Render scene from cube face's point of view
			vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

			// Update shader push constant block
			pushBlock.invView = matrices[f];
			pushBlock.proj = projection;

			vkCmdPushConstants(commandBuffer, tmpPipeline.GetGraphicsPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushBlock), &pushBlock);
			
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, tmpPipeline.GetGraphicsPipeline());

			size_t descrtiptorSetCount = descriptorSets.size();

			for (int index = 0; index < descrtiptorSetCount; index++)
			{
				vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, tmpPipeline.GetGraphicsPipelineLayout(), index, 1, &descriptorSets[index].GetFrameDescriptorSet(0), 0, nullptr);
			}

			VkBuffer vertexBuffers[] = { VBO->GetVkBuffer() };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(commandBuffer, IBO->GetVkBuffer(), 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(_mesh->indiceCount), 1, 0, 0, 0);

			vkCmdEndRenderPass(commandBuffer);

			offscreen.image.TransitionImageLayout(format, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, commandBuffer);

			// Copy region for transfer from framebuffer to cube face
			VkImageCopy copyRegion = {};

			copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			copyRegion.srcSubresource.baseArrayLayer = 0;
			copyRegion.srcSubresource.mipLevel = 0;
			copyRegion.srcSubresource.layerCount = 1;
			copyRegion.srcOffset = { 0, 0, 0 };

			copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			copyRegion.dstSubresource.baseArrayLayer = f;
			copyRegion.dstSubresource.mipLevel = m;
			copyRegion.dstSubresource.layerCount = 1;
			copyRegion.dstOffset = { 0, 0, 0 };

			copyRegion.extent.width = static_cast<uint32_t>(viewport.width);
			copyRegion.extent.height = -static_cast<uint32_t>(viewport.height);
			copyRegion.extent.depth = 1;

			vkCmdCopyImage(
				commandBuffer,
				offscreen.image.GetVkImage(),
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				outputTexture->GetImage()->GetVkImage(),
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&copyRegion);

			offscreen.image.TransitionImageLayout(format, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, commandBuffer);

		}
	}
	outputTexture->GetImage()->TransitionImageLayout(format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, commandBuffer);

	CommandBuffer::EndSingleTimeCommands(logicalDevice, commandPool, graphicsQueue, commandBuffer);

	tmpPipeline.Cleanup();
	offscreen.framebuffer.Cleanup();
	offscreen.image.Cleanup();
	tmpRenderPass.Cleanup();

	_output->iTexture = outputTexture;

	return true;
}

void RenderContext::Cleanup()
{
	for (std::forward_list<SceneData>::iterator it = scenesData.begin(); it != scenesData.end(); ++it)
	{
		it->vkScene.Cleanup();
	}

	windowProperties->window->FramebufferResizeEvent.Remove(this, &RenderContext::FrameBufferResizedCallback);
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		commandBuffers[i].Cleanup();
	}
	frameBuffer.Cleanup();
	depthBuffer.Cleanup();
	colorImage.Cleanup();
	renderPass.Cleanup();
	swapChain.Cleanup();
}