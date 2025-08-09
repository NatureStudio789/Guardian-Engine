#ifndef _GE_GRENDERER_H_
#define _GE_GRENDERER_H_
#include "RenderGraph/GSceneRenderGraph.h"

namespace GE
{
	class GUARDIAN_API GRenderer
	{
	public:
		static void InitializeRender();
		static void RegisterRenderGraph(std::shared_ptr<GRenderGraph> renderGraph);

		static void Render();

		static std::shared_ptr<GSceneRenderGraph> GetSceneRenderGraph();
		static std::shared_ptr<GRenderGraph> GetRenderGraph(const GUUID& id);
		static std::shared_ptr<GRenderGraph> GetRenderGraph(const std::string& name);

		static std::string SCENE_RENDERGRAPH;

	private:
		static std::shared_ptr<GSceneRenderGraph> SceneRenderGraph;
		static std::map<std::string, std::shared_ptr<GRenderGraph>> RenderGraphList;
	};
}

#endif