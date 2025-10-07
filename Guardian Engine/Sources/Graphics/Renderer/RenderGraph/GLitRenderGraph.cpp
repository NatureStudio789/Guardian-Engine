#include "GLitRenderGraph.h"

namespace GE
{
	GLitRenderGraph::GLitRenderGraph(const std::string& name) : GRenderGraph(name)
	{
		this->AddGlobalSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource("Framebuffer", this->RenderGraphFramebuffer));
		this->AddGlobalSink(GDirectFramebufferSink::CreateNewDirectFramebufferSink("Framebuffer", this->RenderGraphFramebuffer));

		this->AddGlobalSource(GDirectCameraSource::CreateNewDirectCameraSource("Camera", this->RenderGraphCamera));
		this->AddGlobalSink(GDirectCameraSink::CreateNewDirectCameraSink("Camera", this->RenderGraphCamera));

		{
			auto ClearFramebufferPass = std::make_shared<GClearFramebufferPass>("Clearing");
			ClearFramebufferPass->SetSinkLinkage("ClearingFramebuffer", "$.Framebuffer");

			this->AppendPass(ClearFramebufferPass);
		}

		{
			auto LightingPass = std::make_shared<GLightingPass>("Lighting");
			LightingPass->SetSinkLinkage("Framebuffer", "Clearing.ClearingFramebuffer");
			LightingPass->SetSinkLinkage("Camera", "$.Camera");
			
			this->AppendPass(LightingPass);
		}

		{
			auto WireframePass = std::make_shared<GWireframePass>("Wireframe");
			WireframePass->SetSinkLinkage("Framebuffer", "Lighting.Framebuffer");
			WireframePass->SetSinkLinkage("Camera", "Lighting.Camera");

			this->AppendPass(WireframePass);
		}
	}
}
