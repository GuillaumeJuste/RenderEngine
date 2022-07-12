#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINECREATEINFO
#define RENDERENGINE_GRAPHICSPIPELINECREATEINFO

#include "Engine/Shader/Shader.hpp"

namespace RenderEngine
{
	struct GraphicsPipelineCreateInfo
	{
		Shader vertexShader;
		Shader fragmentShader;

		GraphicsPipelineCreateInfo() = default;
		GraphicsPipelineCreateInfo(const Shader& _vertexShader,const Shader& _fragmentShader);
	};
}

#endif