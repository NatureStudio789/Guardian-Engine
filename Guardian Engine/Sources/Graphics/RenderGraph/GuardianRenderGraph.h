#ifndef _GE_GUARDIANRENDERGRAPH_H_
#define _GE_GUARDIANRENDERGRAPH_H_
#include "../Renderable/Geometry/GuardianGeometry.h"

namespace GE
{
	class GUARDIAN_API GuardianRenderGraph
	{
	public:
		GuardianRenderGraph();
		GuardianRenderGraph(const GString& graphName, int width, int height);
		GuardianRenderGraph(const GuardianRenderGraph& other);
		virtual ~GuardianRenderGraph();

		void InitializeRenderGraph(const GString& graphName, int width, int height);
		void SetGraphClearColor(const GVector3& color);
		void SetGraphCamera(const GuardianCamera& camera);
		void SetGraphShaderGroup(const GString& groupName);

		void SubmitRenderable(GuardianSubmitPassLevel level, std::shared_ptr<GuardianRenderable> renderable);

		void Render();

		void ResizeRenderGraph(int width, int height);

		std::shared_ptr<GuardianFramebuffer> GetRenderGraphFramebuffer();

	protected:
		GString GraphName;

		GVector3 GraphClearColor;

		std::shared_ptr<GuardianShaderGroup> GraphShaderGroup;
		
		std::shared_ptr<GuardianFramebuffer> GraphFramebuffer;
		GuardianCamera GraphCamera;
		std::map<GuardianSubmitPassLevel, std::queue<std::shared_ptr<GuardianRenderable>>> RenderableQueueList;
	
	private:

		friend class GuardianRenderer;
	};
}

#endif