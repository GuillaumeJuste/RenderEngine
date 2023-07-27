#include "Rendering/Vulkan/Scene/GameObject/VkGameObject.hpp"

#include "Rendering/Vulkan/UniformBuffer/UniformBufferData.hpp"
#include "Rendering/Vulkan/Scene/Data/Material.hpp"
#include "Rendering/Vulkan/Descriptor/Set/DescriptorDataListCreateInfo.hpp"

#include "Misc/Math.hpp"

#include <array>

using namespace RenderEngine::Rendering;

VkGameObject::VkGameObject(const VkGameObjectCreateInfo& _createInfo) :
	createInfo{ _createInfo }
{
	meshRenderer = createInfo.gameObject->GetComponent<MeshRenderer>();
	
	VBO = dynamic_cast<BufferObject*>(meshRenderer->mesh->vertexBuffer);
	IBO = dynamic_cast<BufferObject*>(meshRenderer->mesh->indexBuffer);

	CreateDescriptorBufferObjects();

	CreateGraphicsPipeline();
}

void VkGameObject::CreateGraphicsPipeline()
{
	if (meshRenderer != nullptr)
	{
		GraphicsPipelineVkCreateInfo gpCreateInfo{};
		gpCreateInfo.logicalDevice = createInfo.logicalDevice;
		gpCreateInfo.renderPass = createInfo.renderpass;
		gpCreateInfo.swapChainExtent = createInfo.swapchain->GetExtent();
		gpCreateInfo.swapChainImageFormat = createInfo.swapchain->GetImageFormat();
		gpCreateInfo.vertexShader = dynamic_cast<VkShader*>(meshRenderer->material.vertexShader->iShader);
		gpCreateInfo.fragmentShader = dynamic_cast<VkShader*>(meshRenderer->material.fragmentShader->iShader);
		gpCreateInfo.drawMode = meshRenderer->material.drawMode;
		gpCreateInfo.lineWidth = meshRenderer->material.lineWidth;
		gpCreateInfo.frontFace = meshRenderer->material.frontFace;
		gpCreateInfo.samples = createInfo.samples;

		DescriptorDataListCreateInfo descriptorDataListCreateInfo{};
		descriptorDataListCreateInfo.descriptorSets.push_back(meshRenderer->material.vertexShaderDescriptorSet);
		descriptorDataListCreateInfo.descriptorSets.push_back(meshRenderer->material.fragmentShaderDescriptorSet);
		descriptorDataListCreateInfo.uniformBuffer = &uniformBufferObject;
		descriptorDataListCreateInfo.materialBuffer = &materialBufferObject;
		descriptorDataListCreateInfo.cameraBuffer = createInfo.cameraBuffer;
		descriptorDataListCreateInfo.pointLightBuffer = createInfo.pointLightsBuffer;
		descriptorDataListCreateInfo.directionalLightBuffer = createInfo.directionalLightsBuffer;
		descriptorDataListCreateInfo.spotLightBuffer = createInfo.spotLightsBuffer;
		descriptorDataListCreateInfo.skyboxMap = createInfo.skyboxMap;
		descriptorDataListCreateInfo.irradianceMap = createInfo.irradianceMap;
		descriptorDataListCreateInfo.prefilterMap = createInfo.prefilterMap;
		descriptorDataListCreateInfo.BRDFlut = createInfo.BRDFlut;
		if (meshRenderer != nullptr)
		{
			if(meshRenderer->material.albedo != nullptr)
				descriptorDataListCreateInfo.albedo = dynamic_cast<VkTexture*>(meshRenderer->material.albedo->iTexture);
			if (meshRenderer->material.metalnessMap != nullptr)
				descriptorDataListCreateInfo.metalnessMap = dynamic_cast<VkTexture*>(meshRenderer->material.metalnessMap->iTexture);
			if (meshRenderer->material.roughnessMap != nullptr)
				descriptorDataListCreateInfo.roughnessMap = dynamic_cast<VkTexture*>(meshRenderer->material.roughnessMap->iTexture);
			if (meshRenderer->material.normalMap != nullptr)
				descriptorDataListCreateInfo.normalMap = dynamic_cast<VkTexture*>(meshRenderer->material.normalMap->iTexture);
			if (meshRenderer->material.ambientOcclusionMap != nullptr)
				descriptorDataListCreateInfo.ambientOcclusionMap = dynamic_cast<VkTexture*>(meshRenderer->material.ambientOcclusionMap->iTexture);
		}

		gpCreateInfo.descriptorDatas = DescriptorDataList::GenerateDescriptorDataLists(descriptorDataListCreateInfo);

		GraphicsPipeline::InitalizeGraphicsPipeline(gpCreateInfo, &graphicsPipeline);

		CreateDescriptorSet(gpCreateInfo.descriptorDatas);
	}
}

void VkGameObject::CreateDescriptorBufferObjects()
{
	BufferObjectVkCreateInfo uniformBufferCreateInfo;
	uniformBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	uniformBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	uniformBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	uniformBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	uniformBufferCreateInfo.bufferSize = sizeof(UniformBufferData);

	DescriptorBuffer::InitializeDescriptorBuffer(uniformBufferCreateInfo, MAX_FRAMES_IN_FLIGHT,  &uniformBufferObject);

	BufferObjectVkCreateInfo materialBufferCreateInfo;
	materialBufferCreateInfo.physicalDevice = createInfo.physicalDevice;
	materialBufferCreateInfo.logicalDevice = createInfo.logicalDevice;
	materialBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	materialBufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	materialBufferCreateInfo.bufferSize = sizeof(Material);

	DescriptorBuffer::InitializeDescriptorBuffer(materialBufferCreateInfo, MAX_FRAMES_IN_FLIGHT, &materialBufferObject);
}

void VkGameObject::CreateDescriptorSet(std::vector<DescriptorDataList> _descriptorDatas)
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

void VkGameObject::Draw(VkCommandBuffer _commandBuffer, int _currentFrame) const
{
	if (HasMeshRenderer())
	{
		if (meshRenderer->enable)
		{
			vkCmdBindPipeline(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetGraphicsPipeline());

			VkBuffer vertexBuffers[] = { VBO->GetVkBuffer() };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(_commandBuffer, 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(_commandBuffer, IBO->GetVkBuffer(), 0, VK_INDEX_TYPE_UINT16);
			
			size_t descrtiptorSetCount = descriptorSets.size();

			for (int index = 0; index < descrtiptorSetCount; index++)
			{
				vkCmdBindDescriptorSets(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetGraphicsPipelineLayout(), index, 1, &descriptorSets[index].GetFrameDescriptorSet(_currentFrame), 0, nullptr);
			}
			vkCmdDrawIndexed(_commandBuffer, static_cast<uint32_t>(meshRenderer->mesh->indiceCount), 1, 0, 0, 0);
		}
	}
}

MeshRenderer* VkGameObject::GetMeshRenderer() const
{
	return meshRenderer;
}

bool VkGameObject::HasMeshRenderer() const
{
	return meshRenderer != nullptr;
}

void VkGameObject::Update(size_t _currentframe)
{
	UniformBufferData uboData{};
	
	uboData.model = createInfo.gameObject->GetWorldTransform().ToMatrixWithScale().Transpose();

	uniformBufferObject.CopyDataToBuffer<UniformBufferData>((int)_currentframe, &uboData, sizeof(UniformBufferData));

	Material material{};
	material.ambient = meshRenderer->material.ambient;
	material.diffuse = meshRenderer->material.diffuse;
	material.specular = meshRenderer->material.specular;
	material.shininess = meshRenderer->material.shininess;

	materialBufferObject.CopyDataToBuffer<Material>((int)_currentframe, &material, sizeof(Material));
}

void VkGameObject::Cleanup()
{
	uniformBufferObject.Cleanup();
	materialBufferObject.Cleanup();

	graphicsPipeline.Cleanup();
}