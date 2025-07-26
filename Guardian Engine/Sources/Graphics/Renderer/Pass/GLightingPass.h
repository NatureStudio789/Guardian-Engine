#ifndef _GE_GLIGHTINGPASS_H_
#define _GE_GLIGHTINGPASS_H_
#include "GRenderQueuePass.h"

namespace GE
{
	class GUARDIAN_API GLightingPass : public GRenderQueuePass
	{
	public:
		GLightingPass(const std::string& name) : GRenderQueuePass(name)
		{
			this->RegisterSink(GDirectFramebufferSink::CreateNewDirectFramebufferSink(
				"Framebuffer", this->Framebuffer));
			this->RegisterSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource(
				"Framebuffer", this->Framebuffer));

			this->AddApplicable(GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO));
		}
	};
}

#endif