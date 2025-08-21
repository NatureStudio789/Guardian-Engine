#include "GRenderer.h"

namespace GE
{
	std::string GRenderer::SCENE_RENDERGRAPH = "Scene";
	
	std::shared_ptr<GSceneRenderGraph> GRenderer::SceneRenderGraph;
	std::map<std::string, std::shared_ptr<GRenderGraph>> GRenderer::RenderGraphList;


	void GRenderer::InitializeRender()
	{
		SceneRenderGraph = GSceneRenderGraph::CreateNewSceneRenderGraph(SCENE_RENDERGRAPH);
		RegisterRenderGraph(SceneRenderGraph);
	}

	void GRenderer::RegisterRenderGraph(std::shared_ptr<GRenderGraph> renderGraph)
	{
		if (RenderGraphList.count(renderGraph->GetRenderGraphName()))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The render graph named '{}' already exists in the renderer!",
				renderGraph->GetRenderGraphName()));
		}

		RenderGraphList[renderGraph->GetRenderGraphName()] = renderGraph;
	}

	void GRenderer::Render()
	{
		for (auto& RenderGraph : RenderGraphList)
		{
			RenderGraph.second->Execute();
		}

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->ExecuteCommandListBatch();
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->PresentRenderingResult(true);

		for (auto& RenderGraph : RenderGraphList)
		{
			RenderGraph.second->Reset();
		}
	}

	std::shared_ptr<GSceneRenderGraph> GRenderer::GetSceneRenderGraph()
	{
		return SceneRenderGraph;
	}

	std::shared_ptr<GRenderGraph> GRenderer::GetRenderGraph(const GUUID& id)
	{
		for (auto& renderGraph : RenderGraphList)
		{
			if (renderGraph.second->GetRenderGraphId() == id)
			{
				return renderGraph.second;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No render graph with id '{}' found in render!", (unsigned long long)id));
	}

	std::shared_ptr<GRenderGraph> GRenderer::GetRenderGraph(const std::string& name)
	{
		if (!RenderGraphList.count(name))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No render graph named '{}' found in render!", name));
		}

		return RenderGraphList[name];
	}
}
