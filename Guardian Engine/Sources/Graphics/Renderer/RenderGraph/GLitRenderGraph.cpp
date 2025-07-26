#include "GLitRenderGraph.h"

namespace GE
{
	GLitRenderGraph::GLitRenderGraph(const std::string& name) : GRenderGraph(name)
	{
		this->InitializeGraphGraphics();
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->BeginInitializing();

		{
			auto pass = std::make_shared<GLightingPass>("Lighting");
			pass->SetSinkLinkage("Framebuffer", "$.Framebuffer");
			
			this->AppendPass(pass);
		}

		this->Finalize();

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->EndUpInitializing();
	}
}
