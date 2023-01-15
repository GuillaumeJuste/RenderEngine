#include "Engine/Vulkan/Scene/GameObject/VkGameObject.hpp"

#include "Engine/Vulkan/UniformBuffer/UniformBufferData.hpp"
#include "Engine/Vulkan/Scene/Data/Material.hpp"

#include "Misc/Math.hpp"

#include <array>

using namespace RenderEngine::Engine::Vulkan;

VkGameObject::VkGameObject(const VkGameObjectCreateInfo& _createInfo) :
	createInfo{ _createInfo }
{
	meshRenderer = createInfo.gameObject->GetComponent<MeshRenderer>();
	
	CreateDescriptorBufferObjects();
}

void VkGameObject::CreateGraphicsPipeline(DescriptorBuffer* _cameraBuffer, DescriptorBuffer* _pointLightsBuffer, DescriptorBuffer* _directionalLightsBuffer, DescriptorBuffer* _spotLightsBuffer)
{
	if (meshRenderer != nullptr)
	{
		GraphicsPipelineVkCreateInfo gpCreateInfo{};
		gpCreateInfo.logicalDevice = createInfo.logicalDevice;
		gpCreateInfo.renderPass = createInfo.renderpass;
		gpCreateInfo.swapChainExtent = createInfo.swapchain->GetExtent();
		gpCreateInfo.swapChainImageFormat = createInfo.swapchain->GetImageFormat();
		gpCreateInfo.meshRenderer = meshRenderer;

		gpCreateInfo.descriptorDatas.push_back(GenerateDefaultVertexShaderDescriptorSet(_cameraBuffer));
		gpCreateInfo.descriptorDatas.push_back(GenerateDefaultFragmentShaderDescriptorSet(_pointLightsBuffer, _directionalLightsBuffer, _spotLightsBuffer));

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

	DescriptorBuffer::InitializeDescriptorBuffer(uniformBufferCreateInfo, MAX_FRAMES_IN_FLIGHT, &materialBufferObject);
}

DescriptorDataList VkGameObject::GenerateDefaultVertexShaderDescriptorSet(DescriptorBuffer* _cameraBuffer)
{
	DescriptorDataList datalist{};

	DescriptorData uniformBufferData{};
	uniformBufferData.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uniformBufferData.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uniformBufferData.binding = 0;
	uniformBufferData.buffer = &uniformBufferObject;
	datalist.Add(uniformBufferData);

	DescriptorData cameraBufferData{};
	cameraBufferData.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	cameraBufferData.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	cameraBufferData.binding = 1;
	cameraBufferData.buffer = _cameraBuffer;
	datalist.Add(cameraBufferData);

	return datalist;
}

DescriptorDataList VkGameObject::GenerateDefaultFragmentShaderDescriptorSet(DescriptorBuffer* _pointLightsBuffer, DescriptorBuffer* _directionalLightsBuffer, DescriptorBuffer* _spotLightsBuffer)
{
	DescriptorDataList datalist{};

	DescriptorData textureBufferData{};
	textureBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	textureBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	textureBufferData.binding = 0;
	textureBufferData.texture = &createInfo.textureData->vkTexture;
	datalist.Add(textureBufferData);

	DescriptorData specularBufferData{};
	specularBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	specularBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	specularBufferData.binding = 1;
	specularBufferData.texture = &createInfo.specularMap->vkTexture;
	datalist.Add(specularBufferData);

	DescriptorData materialBufferData{};
	materialBufferData.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	materialBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	materialBufferData.binding = 2;
	materialBufferData.buffer = &materialBufferObject;
	datalist.Add(materialBufferData);

	DescriptorData pointLightBufferData{};
	pointLightBufferData.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	pointLightBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	pointLightBufferData.binding = 3;
	pointLightBufferData.buffer = _pointLightsBuffer;
	datalist.Add(pointLightBufferData);

	DescriptorData directionalLightBufferData{};
	directionalLightBufferData.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	directionalLightBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	directionalLightBufferData.binding = 4;
	directionalLightBufferData.buffer = _directionalLightsBuffer;
	datalist.Add(directionalLightBufferData);

	DescriptorData spotLightBufferData{};
	spotLightBufferData.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	spotLightBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	spotLightBufferData.binding = 5;
	spotLightBufferData.buffer = _spotLightsBuffer;
	datalist.Add(spotLightBufferData);

	return datalist;
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

			VkBuffer vertexBuffers[] = { createInfo.meshData->vertexBufferObject.GetVkBuffer() };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(_commandBuffer, 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(_commandBuffer, createInfo.meshData->indexBufferObject.GetVkBuffer(), 0, VK_INDEX_TYPE_UINT16);
			
			size_t descrtiptorSetCount = descriptorSets.size();

			for (int index = 0; index < descrtiptorSetCount; index++)
			{
				vkCmdBindDescriptorSets(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.GetGraphicsPipelineLayout(), index, 1, &descriptorSets[index].GetFrameDescriptorSet(_currentFrame), 0, nullptr);
			}
			vkCmdDrawIndexed(_commandBuffer, static_cast<uint32_t>(meshRenderer->GetMesh()->indices.size()), 1, 0, 0, 0);
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
	material.shininess = meshRenderer->shininess;
	material.ambient = meshRenderer->ambient;
	material.diffuse = meshRenderer->diffuse;
	material.specular = meshRenderer->specular;

	materialBufferObject.CopyDataToBuffer<Material>((int)_currentframe, &material, sizeof(Material));
}

void VkGameObject::Cleanup()
{
	uniformBufferObject.Cleanup();
	materialBufferObject.Cleanup();

	graphicsPipeline.Cleanup();
}