#include "GLitRenderGraph.h"

namespace GE
{
	GLitRenderGraph::GLitRenderGraph(const std::string& name) : GRenderGraph(name, true)
	{
		{
			auto LightingPass = std::make_shared<GLightingPass>("Lighting");
			LightingPass->SetSinkLinkage("Framebuffer", "$.Framebuffer");
			LightingPass->SetSinkLinkage("Camera", "$.Camera");
			
			this->AppendPass(LightingPass);
		}

		this->Finalize();
	}
}
