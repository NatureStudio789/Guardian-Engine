#ifndef _GE_GUARDIANRENDERER_H_
#define _GE_GUARDIANRENDERER_H_
#include "../RenderGraph/GuardianSceneGraph.h"

namespace GE
{
	class GUARDIAN_API GuardianRenderer
	{
	public:
		static void InitializeRenderer();

		static void CreateRenderingRenderGraph(
			const GString& renderGraphName, int width, int height);
		static void ResizeRenderingRenderGraph(const GString& renderGraphName, int width, int height);
		static void SetRenderingRenderGraphShaderGroup(const GString& renderGraphName, const GString& shaderGroupName);
		static void SetRenderingRenderGraphCamera(const GString& renderGraphName, const GuardianCamera& camera);
		static void CreateRenderingRenderGraphDepthGraph(const GString& renderGraphName, 
			const GString& depthGraphName, int width, int height);
		static std::shared_ptr<GuardianDepthGraph> GetRenderingRenderGraphDepthGraph(const GString& renderGraphName, const GString& depthGraphName);
		static std::shared_ptr<GuardianFramebuffer> GetRenderingRenderGraphFramebuffer(const GString& name);

		static void CreateRenderingSceneGraph(GuardianScene* scene, int width, int height);
		static std::shared_ptr<GuardianSceneGraph> GetRenderingSceneGraph(GuardianScene* scene);

		static void SubmitRenderable(GuardianSubmitPassLevel level, 
			const GString& submitGraphName, std::shared_ptr<GuardianRenderable> renderable);

		static void SetRenderingRenderGraphClearColor(
			const GString& renderGraphName, const GVector3& color);

		static void Render();

	private:
		static std::map<GString, std::shared_ptr<GuardianRenderGraph>> RenderingRenderGraphList;
		static std::map<GString, std::shared_ptr<GuardianSceneGraph>> RenderingSceneGraphList;

		friend class GuardianApplication;
		friend class GuardianScenePanel;
	};
}

#endif