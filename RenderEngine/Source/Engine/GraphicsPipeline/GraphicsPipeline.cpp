#include "Engine/GraphicsPipeline/GraphicsPipeline.hpp"

using namespace RenderEngine;

void GraphicsPipeline::InitalizeGraphicsPipeline(GraphicsPipelineCreateInfo _createInfo)
{
	vertexShader = _createInfo.vertexShader ;
	fragmentShader = _createInfo.fragmentShader;
}

void GraphicsPipeline::Cleanup()
{
	vertexShader.Cleanup();
	fragmentShader.Cleanup();
}