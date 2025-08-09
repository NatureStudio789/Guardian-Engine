#ifndef _GE_GWIREFRAMEPASS_H_
#define _GE_GWIREFRAMEPASS_H_
#include "GRenderQueuePass.h"

namespace GE
{
	class GUARDIAN_API GWireframePass : public GRenderQueuePass
	{
	public:
		GWireframePass(const std::string& name) : GRenderQueuePass(name)
		{
			this->RegisterSink(GDirectFramebufferSink::CreateNewDirectFramebufferSink("Framebuffer", this->Framebuffer));
			this->RegisterSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource("Framebuffer", this->Framebuffer));

			this->RegisterSink(GDirectCameraSink::CreateNewDirectCameraSink("Camera", this->Camera));
			this->RegisterSource(GDirectCameraSource::CreateNewDirectCameraSource("Camera", this->Camera));

			this->AddApplicable(GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::WIREFRAME_PSO));

			this->CameraCBuffer = GCameraCBuffer::CreateNewCameraCBuffer(
				GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::WIREFRAME_PSO)->GetPipelineRootSignature());
			this->AddApplicable(this->CameraCBuffer);
		}

	private:
		std::shared_ptr<GCameraCBuffer> CameraCBuffer;
		std::shared_ptr<GCamera> Camera;
	};
}

#endif