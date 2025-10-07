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
				"SceneFramebuffer", GRenderer::GetRenderGraph("SceneRuntime")->GetRenderGraphFramebuffer()));

			{
				auto ClearFullscreenPass = std::make_shared<GClearFramebufferPass>("Clearing");
				ClearFullscreenPass->SetSinkLinkage("ClearingFramebuffer", "$.ClearingFramebuffer");

				this->AppendPass(ClearFullscreenPass);
			}

			{
				auto FullscreenPass = std::make_shared<GFullscreenPass>("Fullscreen");
				FullscreenPass->SetSinkLinkage("SourceFramebuffer", "$.SceneFramebuffer");
				FullscreenPass->SetSinkLinkage("TargetFramebuffer", "Clearing.ClearingFramebuffer");

				this->AppendPass(FullscreenPass);
			}
		}

	private:

	};
}

#endif