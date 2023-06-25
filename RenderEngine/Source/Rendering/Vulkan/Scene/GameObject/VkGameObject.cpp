#include "Rendering/Vulkan/Scene/GameObject/VkGameObject.hpp"

#include "Rendering/Vulkan/UniformBuffer/UniformBufferData.hpp"
#include "Rendering/Vulkan/Scene/Data/Material.hpp"

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
		gpCreateInfo.vertexShader = meshRenderer->vertexShader;
		gpCreateInfo.fragmentShader = meshRenderer->fragmentShader;
		gpCreateInfo.drawMode = meshRenderer->drawMode;
		gpCreateInfo.lineWidth = meshRenderer->lineWidth;
		gpCreateInfo.frontFace = meshRenderer->frontFace;
		gpCreateInfo.samples = createInfo.samples;

		gpCreateInfo.descriptorDatas.push_back(GenerateDefaultVertexShaderDescriptorSet());
		gpCreateInfo.descriptorDatas.push_back(GenerateDefaultFragmentShaderDescriptorSet());

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

DescriptorDataList VkGameObject::GenerateDefaultVertexShaderDescriptorSet()
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
	cameraBufferData.buffer = createInfo.cameraBuffer;
	datalist.Add(cameraBufferData);

	return datalist;
}

DescriptorDataList VkGameObject::GenerateDefaultFragmentShaderDescriptorSet()
{
	DescriptorDataList datalist{};

	if (meshRenderer != nullptr)
	{
		DescriptorData textureBufferData{};
		textureBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		textureBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		textureBufferData.binding = 0;
		textureBufferData.texture = dynamic_cast<VkTexture*>(meshRenderer->texture->iTexture);
		datalist.Add(textureBufferData);

		DescriptorData metalnessMapBufferData{};
		metalnessMapBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		metalnessMapBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		metalnessMapBufferData.binding = 1;
		metalnessMapBufferData.texture = dynamic_cast<VkTexture*>(meshRenderer->metalnessMap->iTexture);
		datalist.Add(metalnessMapBufferData);

		DescriptorData roughnessMapBufferData{};
		roughnessMapBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		roughnessMapBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		roughnessMapBufferData.binding = 2;
		roughnessMapBufferData.texture = dynamic_cast<VkTexture*>(meshRenderer->roughnessMap->iTexture);
		datalist.Add(roughnessMapBufferData);

		DescriptorData aoMapBufferData{};
		aoMapBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		aoMapBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		aoMapBufferData.binding = 3;
		aoMapBufferData.texture = dynamic_cast<VkTexture*>(meshRenderer->ambientOcclusionMap->iTexture);
		datalist.Add(aoMapBufferData);
	}

	DescriptorData materialBufferData{};
	materialBufferData.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	materialBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	materialBufferData.binding = 4;
	materialBufferData.buffer = &materialBufferObject;
	datalist.Add(materialBufferData);

	DescriptorData pointLightBufferData{};
	pointLightBufferData.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	pointLightBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	pointLightBufferData.binding = 5;
	pointLightBufferData.buffer = createInfo.pointLightsBuffer;
	datalist.Add(pointLightBufferData);

	DescriptorData directionalLightBufferData{};
	directionalLightBufferData.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	directionalLightBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	directionalLightBufferData.binding = 6;
	directionalLightBufferData.buffer = createInfo.directionalLightsBuffer;
	datalist.Add(directionalLightBufferData);

	DescriptorData spotLightBufferData{};
	spotLightBufferData.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	spotLightBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	spotLightBufferData.binding = 7;
	spotLightBufferData.buffer = createInfo.spotLightsBuffer;
	datalist.Add(spotLightBufferData);

	DescriptorData irradianceBufferData{};
	irradianceBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	irradianceBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	irradianceBufferData.binding = 8;
	irradianceBufferData.texture = createInfo.irradianceMap;
	datalist.Add(irradianceBufferData);

	DescriptorData prefilterBufferData{};
	prefilterBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	prefilterBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	prefilterBufferData.binding = 9;
	prefilterBufferData.texture = createInfo.prefilterMap;
	datalist.Add(prefilterBufferData);

	DescriptorData brdflutBufferData{};
	brdflutBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	brdflutBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	brdflutBufferData.binding = 10;
	brdflutBufferData.texture = createInfo.BRDFlut;
	datalist.Add(brdflutBufferData);

	DescriptorData skyboxBufferData{};
	skyboxBufferData.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	skyboxBufferData.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	skyboxBufferData.binding = 11;
	skyboxBufferData.texture = createInfo.skyboxMap;
	datalist.Add(skyboxBufferData);

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