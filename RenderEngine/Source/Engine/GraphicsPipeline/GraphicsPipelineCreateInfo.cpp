#include "Engine/GraphicsPipeline/GraphicsPipelineCreateInfo.hpp"

using namespace RenderEngine;

GraphicsPipelineCreateInfo::GraphicsPipelineCreateInfo(Shader _vertexShader, Shader _fragmentShader) :
	vertexShader { _vertexShader }, fragmentShader { _fragmentShader}
{
}