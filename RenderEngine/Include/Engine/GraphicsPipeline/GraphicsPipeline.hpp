#pragma once

#ifndef RENDERENGINE_GRAPHICSPIPELINE
#define RENDERENGINE_GRAPHICSPIPELINE

#include "Engine/GraphicsPipeline/GraphicsPipelineCreateInfo.hpp"

namespace RenderEngine
{
	class GraphicsPipeline
	{
	private:
		Shader vertexShader;
		Shader fragmentShader;

	public:
		GraphicsPipeline() = default;
		~GraphicsPipeline() = default;

		void InitalizeGraphicsPipeline(GraphicsPipelineCreateInfo _createInfo);
		void Cleanup();
	};

}

#endif