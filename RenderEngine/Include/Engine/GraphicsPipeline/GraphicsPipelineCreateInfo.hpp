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
		GraphicsPipelineCreateInfo(Shader _vertexShader, Shader _fragmentShader);
	};
}

#endif