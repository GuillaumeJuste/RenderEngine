#include "Engine/GraphicsPipeline/GraphicsPipelineCreateInfo.hpp"

using namespace RenderEngine;

GraphicsPipelineCreateInfo::GraphicsPipelineCreateInfo(const Shader& _vertexShader, const Shader& _fragmentShader) :
	vertexShader { _vertexShader }, fragmentShader { _fragmentShader}
{
}