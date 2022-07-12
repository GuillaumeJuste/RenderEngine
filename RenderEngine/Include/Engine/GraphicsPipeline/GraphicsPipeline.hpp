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

		static void InitalizeGraphicsPipeline(GraphicsPipelineCreateInfo _createInfo, GraphicsPipeline* _output);
		void Cleanup();
	};

}

#endif