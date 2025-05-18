#ifndef _GE_GUARDIANDEPTHGRAPH_H_
#define _GE_GUARDIANDEPTHGRAPH_H_
#include "../Renderable/Geometry/GuardianGeometry.h"

namespace GE
{
	class GUARDIAN_API GuardianDepthGraph
	{
	public:
		GuardianDepthGraph();
		GuardianDepthGraph(const GString& graphName, int width, int height);
		GuardianDepthGraph(const GuardianDepthGraph& other);
		virtual ~GuardianDepthGraph();

		void InitializeDepthGraph(const GString& graphName, int width, int height);
		void SetGraphCamera(const GuardianCamera& camera);

		void SubmitRenderable(std::shared_ptr<GuardianRenderable> renderable);

		void Render();

		void ResizeDepthGraph(int width, int height);

		std::shared_ptr<GuardianFramebuffer> GetGraphFramebuffer();
		std::shared_ptr<GuardianDepthStencil> GetGraphDepthStencil();

		static std::shared_ptr<GuardianSampler> DepthMapSampler;

	protected:
		GString GraphName;
		
		std::shared_ptr<GuardianFramebuffer> GraphFramebuffer;
		GuardianCamera GraphCamera;
		std::queue<std::shared_ptr<GuardianRenderable>> RenderableQueue;

	private:
		friend class GuardianRenderGraph;
		friend class GuardianRenderer;
	};
}

#endif