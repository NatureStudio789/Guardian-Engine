#ifndef _GE_GRUNTIMERENDERGRAPH_H_
#define _GE_GRUNTIMERENDERGRAPH_H_
#include <GEngine.h>

namespace GE
{
	class GRuntimeRenderGraph : public GRenderGraph
	{
	public:
		GRuntimeRenderGraph(const std::string& name) : GRenderGraph(name)
		{
			this->AddGlobalSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource(
				"ClearingFramebuffer", this->RenderGraphFramebuffer));
			this->AddGlobalSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource(
				"SceneFramebuffer", GRenderer::GetSceneRenderGraph()->GetRuntimeFramebuffer()));

			{
				auto ClearFullscreenPass = std::make_shared<GClearFramebufferPass>("Clear Fullscreen");
				ClearFullscreenPass->SetSinkLinkage("ClearingFramebuffer", "$.ClearingFramebuffer");

				this->AppendPass(ClearFullscreenPass);
			}

			{
				auto FullscreenPass = std::make_shared<GFullscreenPass>("Fullscreen");
				FullscreenPass->SetSinkLinkage("SourceFramebuffer", "$.SceneFramebuffer");
				FullscreenPass->SetSinkLinkage("TargetFramebuffer", "$.Framebuffer");

				this->AppendPass(FullscreenPass);
			}

			this->Finalize();
		}

	private:

	};
}

#endif