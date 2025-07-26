#ifndef _GE_GRENDERER_H_
#define _GE_GRENDERER_H_
#include "RenderGraph/GLitRenderGraph.h"

namespace GE
{
	class GUARDIAN_API GRenderer
	{
	public:
		static void RegisterRenderGraph(std::shared_ptr<GRenderGraph> renderGraph);

		static void Render();

		static std::shared_ptr<GRenderGraph> GetRenderGraph(const GUUID& id);
		static std::shared_ptr<GRenderGraph> GetRenderGraph(const std::string& name);

	private:
		static std::map<std::string, std::shared_ptr<GRenderGraph>> RenderGraphList;
	};
}

#endif