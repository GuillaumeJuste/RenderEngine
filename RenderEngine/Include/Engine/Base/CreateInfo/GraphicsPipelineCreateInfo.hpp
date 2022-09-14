#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINECREATEINFO
#define RENDERENGINE_GRAPHICSPIPELINECREATEINFO

#include <string>

#include "Engine/Base/Enum/PolygonDrawMode.hpp"
#include "Engine/Base/Enum/FrontFace.hpp"

namespace RenderEngine::Engine::Base
{
	struct GraphicsPipelineCreateInfo
	{
		std::string vertexShaderFilePath;
		std::string fragmentShaderFilePath;
		PolygonDrawMode drawMode = PolygonDrawMode::FILL;
		FrontFace frontFace = FrontFace::CLOCKWISE;
	};
}

#endif