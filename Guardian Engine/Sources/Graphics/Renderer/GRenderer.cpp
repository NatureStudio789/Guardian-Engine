#include "GRenderer.h"

namespace GE
{
	std::vector<GMission*> GRenderer::RenderMissionList;
	std::map<std::string, std::shared_ptr<GRenderGraph>> GRenderer::RenderGraphList;


	void GRenderer::RegisterRenderGraph(std::shared_ptr<GRenderGraph> renderGraph)
	{
		if (RenderGraphList.count(renderGraph->GetRenderGraphName()))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The render graph named '{}' already exists in the renderer!",
				renderGraph->GetRenderGraphName()));
		}

		RenderGraphList[renderGraph->GetRenderGraphName()] = renderGraph;
	}

	void GRenderer::Accept(GMission* mission)
	{
		RenderMissionList.push_back(mission);

		if (!RenderGraphList.count(mission->GetTargetRenderGraphName()))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No render graph named '{}' found in renderer!", mission->GetTargetRenderGraphName()));
		}

		auto& RenderGraph = RenderGraphList[mission->GetTargetRenderGraphName()];

		RenderGraph->SetFramebuffer(mission->GetRenderFramebuffer());
		RenderGraph->Finalize();
	}

	void GRenderer::Render()
	{
		for (auto& mission : RenderMissionList)
		{
			auto& RenderGraph = RenderGraphList[mission->GetTargetRenderGraphName()];
			RenderGraph->SetCamera(mission->GetRenderCamera());

			RenderGraph->Execute();
		}

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->ExecuteCommandListBatch();
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->PresentRenderingResult(true);

		for (auto& mission : RenderMissionList)
		{
			RenderGraphList[mission->GetTargetRenderGraphName()]->Reset();
		}
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
