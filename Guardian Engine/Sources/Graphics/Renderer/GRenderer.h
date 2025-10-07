#ifndef _GE_GRENDERER_H_
#define _GE_GRENDERER_H_
#include "Mission/GMission.h"

namespace GE
{
	class GUARDIAN_API GRenderer
	{
	public:
		static void RegisterRenderGraph(std::shared_ptr<GRenderGraph> renderGraph);

		static void Accept(GMission* mission);
		static void ResizeRenderGraph(const GUUID& id, int newWidth, int newHeight);
		static void ResizeRenderGraph(const std::string& name, int newWidth, int newHeight);
		static void Render();

		static std::shared_ptr<GRenderGraph> GetRenderGraph(const GUUID& id);
		static std::shared_ptr<GRenderGraph> GetRenderGraph(const std::string& name);

	private:
		static std::vector<GMission*> RenderMissionList;
		static std::map<std::string, std::shared_ptr<GRenderGraph>> RenderGraphList;

		static std::queue<GRenderGraphResizeEvent> ResizeEventList;
		static std::shared_ptr<GEventProcesser> RendererEventProcesser;
		static std::shared_ptr<GEventDispatcher> RendererEventDispatcher;
	};
}

#endif