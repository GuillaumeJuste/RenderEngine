#include "Rendering/Vulkan/Scene/Skybox/VkSkybox.hpp"
#include "SceneGraph/Scene/Skybox.hpp"
#include "Rendering/Vulkan/BufferObject/BufferObject.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorDataListCreateInfo.hpp"

using namespace RenderEngine::Rendering;

void VkSkybox::InitializeSkybox(const VkSkyboxCreateInfo& _createInfo, DescriptorBuffer* _cameraBuffer)
{
	createInfo = _createInfo;
	vbo = dynamic_cast<BufferObject*>(createInfo.skybox->mesh->vertexBuffer);
	ibo = dynamic_cast<BufferObject*>(createInfo.skybox->mesh->indexBuffer);
	cubemap = dynamic_cast<VkTexture*>(createInfo.skybox->cubemap->iTexture);
	irradiance = dynamic_cast<VkTexture*>(createInfo.skybox->irradianceMap->iTexture);
	prefilter = dynamic_cast<VkTexture*>(createInfo.skybox->prefilterMap->iTexture);
	BRDFlut = dynamic_cast<VkTexture*>(createInfo.skybox->BRDFlut->iTexture);
	
	CreateGraphicsPipeline(_cameraBuffer);
}

void VkSkybox::CreateGraphicsPipeline(DescriptorBuffer* _cameraBuffer)
{
	GraphicsPipelineVkCreateInfo gpCreateInfo{};
	gpCreateInfo.logicalDevice = createInfo.logicalDevice;
	gpCreateInfo.renderPass = createInfo.renderpass;
	gpCreateInfo.swapChainExtent = createInfo.swapchain->GetExtent();
	gpCreateInfo.swapChainImageFormat = createInfo.swapchain->GetImageFormat();
	gpCreateInfo.vertexShader = dynamic_cast<VkShader*>(createInfo.skybox->vertexShader->iShader);
	gpCreateInfo.fragmentShader = dynamic_cast<VkShader*>(createInfo.skybox->fragmentShader->iShader);
	gpCreateInfo.drawMode = PolygonDrawMode::FILL;
	gpCreateInfo.lineWidth = 1.0f;
	gpCreateInfo.frontFace = FrontFace::COUNTER_CLOCKWISE;
	gpCreateInfo.writeDepthBuffer = VK_FALSE;
	gpCreateInfo.culling_mode = VK_CULL_MODE_FRONT_BIT;
	gpCreateInfo.samples = createInfo.samples;

	DescriptorDataListCreateInfo descriptorDataListCreateInfo{};
	descriptorDataListCreateInfo.descriptorSets.push_back(createInfo.skybox->vertexShaderDescriptorSet);
	descriptorDataListCreateInfo.descriptorSets.push_back(createInfo.skybox->fragmentShaderDescriptorSet);
	descriptorDataListCreateInfo.cameraBuffer = _cameraBuffer;
	descriptorDataListCreateInfo.skyboxMap = cubemap;
	descriptorDataListCreateInfo.irradianceMap = irradiance;
	descriptorDataListCreateInfo.prefilterMap = prefilter;
	descriptorDataListCreateInfo.BRDFlut = BRDFlut;

	gpCreateInfo.descriptorDatas = DescriptorDataList::GenerateDescriptorDataLists(descriptorDataListCreateInfo);
	gpCreateInfo.pushConstants = GraphicsPipeline::GeneratePushConstants(descriptorDataListCreateInfo.descriptorSets);

	GraphicsPipeline::InitalizeGraphicsPipeline(gpCreateInfo, &graphicsPipeline);

	CreateDescriptorSet(gpCreateInfo.descriptorDatas);
}

void VkSkybox::CreateDescriptorSet(std::vector<DescriptorDataList> _descriptorDatas)
{
	size_t descrtiptorSetCount = _descriptorDatas.size();
	descriptorSets.resize(descrtiptorSetCount);

	for (int index = 0; index < descrtiptorSetCount; index++)
	{
		DescriptorSetVkCreateInfo descriptorSetCreateInfo{};
		descriptorSetCreateInfo.logicalDevice = createInfo.logicalDevice;
		descriptorSetCreateInfo.descriptorSetLayout = graphicsPipeline.GetDescriptorSetLayout(index);
		descriptorSetCreateInfo.descriptorPool = graphicsPipeline.GetDescriptorPool(index);
		descriptorSetCreateInfo.descriptorDatas = _descriptorDatas[index];
		descriptorSetCreateInfo.frameCount = MAX_FRAMES_IN_FLIGHT;

		DescriptorSet::InitializeDescriptorSet(descriptorSetCreateInfo, &descriptorSets[index]);
	}
}

void VkSkybox::Draw(VkCommandBuffer _commandBuffer, int _currentFrame) const
{
	vkCmdBindPipeline(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetGraphicsPipeline());
	
	VkBuffer vertexBuffers[] = { vbo->GetVkBuffer()};
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(_commandBuffer, 0, 1, vertexBuffers, offsets);
	
	vkCmdBindIndexBuffer(_commandBuffer, ibo->GetVkBuffer(), 0, VK_INDEX_TYPE_UINT16);
	
	size_t descrtiptorSetCount = descriptorSets.size();
	
	for (int index = 0; index < descrtiptorSetCount; index++)
	{
		vkCmdBindDescriptorSets(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetGraphicsPipelineLayout(), index, 1, &descriptorSets[index].GetFrameDescriptorSet(_currentFrame), 0, nullptr);
	}
	vkCmdDrawIndexed(_commandBuffer, static_cast<uint32_t>(createInfo.skybox->mesh->indiceCount), 1, 0, 0, 0);
}

void VkSkybox::Clean()
{
	graphicsPipeline.Cleanup();
}

VkTexture* VkSkybox::GetCubemap() const
{
	return cubemap;
}

VkTexture* VkSkybox::GetIrradianceCubeMap() const
{
	return irradiance;
}

VkTexture* VkSkybox::GetPrefilterCubemap() const
{
	return prefilter;
}

VkTexture* VkSkybox::GetBRDFlut() const
{
	return BRDFlut;
}