#ifndef _GE_GUARDIANRENDERER_H_
#define _GE_GUARDIANRENDERER_H_
#include "../RenderGraph/GuardianRenderGraph.h"

namespace GE
{
	class GUARDIAN_API GuardianRenderer
	{
	public:
		static void CreateRenderingRenderGraph(
			const GString& renderGraphName, int width, int height);
		static void ResizeRenderingRenderGraph(const GString& renderGraphName, int width, int height);
		static void SetRenderingRenderGraphShaderGroup(const GString& renderGraphName, const GString& shaderGroupName);
		static void SetRenderingRenderGraphCamera(const GString& renderGraphName, const GuardianCamera& camera);
		static std::shared_ptr<GuardianFramebuffer> GetRenderingRenderGraphFramebuffer(const GString& name);

		static void SubmitRenderable(GuardianSubmitPassLevel level, 
			const GString& submitRenderGraph, std::shared_ptr<GuardianRenderable> renderable);

		static void SetRenderingRenderGraphClearColor(
			const GString& renderGraphName, const GVector3& color);

		static void Render();

	private:
		static std::map<GString, std::shared_ptr<GuardianRenderGraph>> RenderingRenderGraphList;

		friend class GuardianApplication;
	};
}

#endif