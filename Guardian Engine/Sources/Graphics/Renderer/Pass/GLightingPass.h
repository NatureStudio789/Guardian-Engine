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

			this->RegisterSink(GDirectCameraSink::CreateNewDirectCameraSink(
				"Camera", this->Camera));
			this->RegisterSource(GDirectCameraSource::CreateNewDirectCameraSource(
				"Camera", this->Camera));

			this->AddApplicable(GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO));

			this->CameraCBuffer = GCameraCBuffer::CreateNewCameraCBuffer(
				GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature());
			this->AddApplicable(this->CameraCBuffer);
		}

		void Execute() override
		{
			GMatrix CameraMatrix = this->Camera->GetViewMatrix() * this->Camera->Projection.GetProjectionMatrix();
			CameraMatrix.Transpose();
			this->CameraCBuffer->UpdateBufferData({ CameraMatrix });

			GRenderQueuePass::Execute();
		}

	private:
		std::shared_ptr<GCameraCBuffer> CameraCBuffer;
		std::shared_ptr<GCamera> Camera;
	};
}

#endif