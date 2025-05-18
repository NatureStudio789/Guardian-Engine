#ifndef _GE_GUARDIANSCENEGRAPH_H_
#define _GE_GUARDIANSCENEGRAPH_H_
#include "GuardianRenderGraph.h"

namespace GE
{
	class GuardianScene;

	class GUARDIAN_API GuardianSceneGraph
	{
	public:
		GuardianSceneGraph();
		GuardianSceneGraph(GuardianScene* scene, int width, int height);
		GuardianSceneGraph(const GuardianSceneGraph& other);
		virtual ~GuardianSceneGraph();

		void InitializeSceneGraph(GuardianScene* scene, int width, int height);
		void SetGraphClearColor(const GVector3& color);
		void SetGraphShaderGroup(const GString& groupName);

		void AddDepthGraph(const GString& name, int width, int height);
		void SetDepthGraphCamera(const GString& name, const GuardianCamera& camera);
		const bool HasDepthGraph(const GString& name) const noexcept;
		std::shared_ptr<GuardianDepthGraph> GetDepthGraph(const GString& name);
		std::shared_ptr<GuardianDepthStencil> GetDepthGraphDepthStencil(const GString& name);

		void SubmitRenderable(GuardianSubmitPassLevel level, std::shared_ptr<GuardianRenderable> renderable);

		void Render();

		void ResizeSceneGraph(int width, int height);

		std::shared_ptr<GuardianFramebuffer> GetSceneGraphFramebuffer();

	protected:
		GuardianScene* GraphScene;

		GVector3 GraphClearColor;
		std::shared_ptr<GuardianShaderGroup> GraphShaderGroup;
		std::shared_ptr<GuardianFramebuffer> GraphFramebuffer;
		GuardianCamera GraphCamera;
		std::map<GuardianSubmitPassLevel, std::queue<std::shared_ptr<GuardianRenderable>>> RenderableQueueList;

		std::map<GString, std::shared_ptr<GuardianDepthGraph>> DepthGraphList;

	private:
		void RenderEditScene();
		void RenderRuntimeScene();

		friend class GuardianRenderer;
		friend class GuardianScenePanel;
	};
}

#endif