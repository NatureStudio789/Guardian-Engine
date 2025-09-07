#include "GLitRenderGraph.h"

namespace GE
{
	GLitRenderGraph::GLitRenderGraph(const std::string& name) : GRenderGraph(name, true)
	{
		this->AddGlobalSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource("ClearingFramebuffer", this->RenderGraphFramebuffer));
		this->AddGlobalSink(GDirectFramebufferSink::CreateNewDirectFramebufferSink("ClearingFramebuffer", this->RenderGraphFramebuffer));

		{
			auto ClearFramebufferPass = std::make_shared<GClearFramebufferPass>("Clear Lighting");
			ClearFramebufferPass->SetSinkLinkage("ClearingFramebuffer", "$.ClearingFramebuffer");

			this->AppendPass(ClearFramebufferPass);
		}

		{
			auto LightingPass = std::make_shared<GLightingPass>("Lighting");
			LightingPass->SetSinkLinkage("Framebuffer", "$.Framebuffer");
			LightingPass->SetSinkLinkage("Camera", "$.Camera");
			
			this->AppendPass(LightingPass);
		}

		this->Finalize();
	}
}
