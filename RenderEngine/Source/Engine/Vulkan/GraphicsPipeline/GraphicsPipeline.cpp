#include "Engine/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "ResourceManager/Assets/Mesh/Vertex.hpp"

#include <iostream>
#include <array>

using namespace RenderEngine::Engine::Vulkan;
using namespace RenderEngine::Assets;

void GraphicsPipeline::InitalizeGraphicsPipeline(const GraphicsPipelineVkCreateInfo& _createInfo, GraphicsPipeline* _output)
{
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->swapChainImageFormat = _createInfo.swapChainImageFormat;
	_output->swapChainExtent = _createInfo.swapChainExtent;
    _output->renderPass = _createInfo.renderPass;
	
    _output->CreateShaders(_createInfo.meshRenderer->vertexShaderFilePath, _createInfo.meshRenderer->fragmentShaderFilePath);

    VkPipelineShaderStageCreateInfo shaderStages[] = { _output->vertexShader.GetShaderStageInfo(), _output->fragmentShader.GetShaderStageInfo() };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    auto bindingDescription = GetVertexBindingDescription();
    auto attributeDescriptions = GetVertexAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = (VkPolygonMode)_createInfo.meshRenderer->drawMode;
    rasterizer.lineWidth = _createInfo.meshRenderer->lineWidth;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = (VkFrontFace)_createInfo.meshRenderer->frontFace;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;


    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    size_t descrtiptorSetCount = _createInfo.descriptorDatas.size();
    _output->descriptorSetLayout.resize(descrtiptorSetCount);

    std::vector<VkDescriptorSetLayout> vkDescriptorSetLayout;

    for (int index = 0; index < descrtiptorSetCount; index++)
    {
        DescriptorSetLayoutVkCreateInfo createInfo{};
        createInfo.logicalDevice = _createInfo.logicalDevice;
        createInfo.descriptorDatas = _createInfo.descriptorDatas[index];

        DescriptorSetLayout::InitializeDescriptorSetLayout(createInfo, &_output->descriptorSetLayout[index]);
        vkDescriptorSetLayout.push_back(_output->descriptorSetLayout[index].GetDescriptorSetLayout());
    }
    

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = (uint32_t)descrtiptorSetCount;
    pipelineLayoutInfo.pSetLayouts = vkDescriptorSetLayout.data();

    if (vkCreatePipelineLayout(_output->logicalDevice, &pipelineLayoutInfo, nullptr, &_output->pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.pDepthStencilState = &depthStencil; 
    pipelineInfo.layout = _output->pipelineLayout;
    pipelineInfo.renderPass = _output->renderPass->GetRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(_output->logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_output->graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    _output->CreateDescriptorPool(_createInfo.descriptorDatas);
}

VkVertexInputBindingDescription GraphicsPipeline::GetVertexBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> GraphicsPipeline::GetVertexAttributeDescriptions()
{
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, normal);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, textCoord);

    return attributeDescriptions;
}

void GraphicsPipeline::CreateShaders(const std::string& _vertexShaderFilePath, const std::string& _fragmentShaderFilePath)
{
    ShaderVkCreateInfo vertexShaderCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, _vertexShaderFilePath, logicalDevice);
    ShaderVkCreateInfo fragmentShaderCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, _fragmentShaderFilePath, logicalDevice);

    Shader::CreateShader(vertexShaderCreateInfo, &vertexShader);
    Shader::CreateShader(fragmentShaderCreateInfo, &fragmentShader);
}

void GraphicsPipeline::CreateDescriptorPool(std::vector<DescriptorDataList> _descriptorSetDatas)
{
    size_t descrtiptorSetCount = _descriptorSetDatas.size();
    descriptorPool.resize(descrtiptorSetCount);

    for (int index = 0; index < descrtiptorSetCount; index++)
    {
        DescriptorPoolVkCreateInfo poolCreateInfo{};
        poolCreateInfo.logicalDevice = logicalDevice;
        poolCreateInfo.frameCount = MAX_FRAMES_IN_FLIGHT;
        poolCreateInfo.descriptorDatas = _descriptorSetDatas[index];

        DescriptorPool::InitializeDescriptorPool(poolCreateInfo, &descriptorPool[index]);
    }
}

void GraphicsPipeline::Cleanup()
{
	vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
    size_t descriptorCount = descriptorSetLayout.size();
    for (size_t index = 0; index < descriptorCount; index++)
    {
        descriptorPool[index].Cleanup();
        descriptorSetLayout[index].Cleanup();

    }
    vertexShader.Cleanup();
	fragmentShader.Cleanup(); 
	std::cout << "[Cleaned] Graphics Pipeline" << std::endl;
}

const VkPipeline& GraphicsPipeline::GetGraphicsPipeline() const
{
	return graphicsPipeline;
}

const VkPipelineLayout& GraphicsPipeline::GetGraphicsPipelineLayout() const
{
    return pipelineLayout;
}

const DescriptorSetLayout& GraphicsPipeline::GetDescriptorSetLayout(size_t _index) const
{
    return descriptorSetLayout[_index];
}

const DescriptorPool& GraphicsPipeline::GetDescriptorPool(size_t _index) const
{
    return descriptorPool[_index];
}