#include "Engine/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp"
#include "Core/Vertrex/Vertex.hpp"

#include <iostream>

using namespace RenderEngine::Engine::Vulkan;
using namespace RenderEngine::Core;

void GraphicsPipeline::InitalizeGraphicsPipeline(const GraphicsPipelineVkCreateInfo& _createInfo, GraphicsPipeline* _output)
{
	_output->logicalDevice = _createInfo.logicalDevice;
	_output->swapChainImageFormat = _createInfo.swapChainImageFormat;
	_output->swapChainExtent = _createInfo.swapChainExtent;
    _output->renderPass = _createInfo.renderPass;
	
    _output->CreateShaders(_createInfo.graphicsPipelineCreateInfo.vertexShaderFilePath, _createInfo.graphicsPipelineCreateInfo.fragmentShaderFilePath);

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
    rasterizer.polygonMode = (VkPolygonMode)_createInfo.graphicsPipelineCreateInfo.drawMode;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = (VkFrontFace)_createInfo.graphicsPipelineCreateInfo.frontFace;
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

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    DescriptorSetLayoutVkCreateInfo createInfo{};
    createInfo.logicalDevice = _createInfo.logicalDevice;

    DescriptorSetLayout::InitializeDescriptorSetLayout(createInfo, &_output->descriptorSetLayout);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &_output->descriptorSetLayout.GetDescriptorSetLayout();

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
    pipelineInfo.layout = _output->pipelineLayout;
    pipelineInfo.renderPass = _output->renderPass->GetRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(_output->logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_output->graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
}

VkVertexInputBindingDescription GraphicsPipeline::GetVertexBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 2> GraphicsPipeline::GetVertexAttributeDescriptions()
{
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
}

void GraphicsPipeline::CreateShaders(const std::string& _vertexShaderFilePath, const std::string& _fragmentShaderFilePath)
{
    ShaderVkCreateInfo vertexShaderCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, _vertexShaderFilePath, logicalDevice);
    ShaderVkCreateInfo fragmentShaderCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, _fragmentShaderFilePath, logicalDevice);

    Shader::CreateShader(vertexShaderCreateInfo, &vertexShader);
    Shader::CreateShader(fragmentShaderCreateInfo, &fragmentShader);
}

void GraphicsPipeline::Cleanup()
{
	vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
    descriptorSetLayout.Cleanup();
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

const DescriptorSetLayout& GraphicsPipeline::GetDescriptorSetLayout() const
{
    return descriptorSetLayout;
}