#ifndef _GE_GLIGHTINGPASS_H_
#define _GE_GLIGHTINGPASS_H_
#include "GDepthPass.h"

namespace GE
{
	class GUARDIAN_API GSceneRegistry;
	
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

			this->LightCBuffer = GLightCBuffer::CreateNewLightCBuffer(
				GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature());
			this->AddApplicable(this->LightCBuffer);

			this->AddApplicable(GShaderViewApplicator::CreateNewShaderViewApplicator("PointLightDepthMapGroup"));
		}

		void Execute() override
		{
			GMatrix CameraMatrix = this->Camera->GetViewMatrix() * this->Camera->Projection.GetProjectionMatrix();
			CameraMatrix.Transpose();
			this->CameraCBuffer->UpdateBufferData({ CameraMatrix, this->Camera->Position });

			this->UpdateLightCBuffer();

			GRenderQueuePass::Execute();
		}

	private:
		void UpdateLightCBuffer();

		std::shared_ptr<GCameraCBuffer> CameraCBuffer;
		std::shared_ptr<GCamera> Camera;

		std::shared_ptr<GLightCBuffer> LightCBuffer;
	};
}

#endif