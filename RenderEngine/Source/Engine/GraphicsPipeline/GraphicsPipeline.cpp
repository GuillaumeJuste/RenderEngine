#include "Engine/GraphicsPipeline/GraphicsPipeline.hpp"

using namespace RenderEngine;

void GraphicsPipeline::InitalizeGraphicsPipeline(GraphicsPipelineCreateInfo _createInfo, GraphicsPipeline* _output)
{
	_output->vertexShader = _createInfo.vertexShader ;
	_output->fragmentShader = _createInfo.fragmentShader;
}

void GraphicsPipeline::Cleanup()
{
	vertexShader.Cleanup();
	fragmentShader.Cleanup();
}