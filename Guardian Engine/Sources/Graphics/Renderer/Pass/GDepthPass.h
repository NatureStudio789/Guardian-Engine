#ifndef _GE_GDEPTHPASS_H_
#define _GE_GDEPTHPASS_H_
#include "GFullscreenPass.h"

namespace GE
{
	class GUARDIAN_API GDepthPass : public GRenderQueuePass
	{
	public:
		GDepthPass(const std::string& name) : GRenderQueuePass(name)
		{
			this->AddApplicable(GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::DEPTH_PSO));

			/*this->DepthJobList.resize(50);
			for (UINT i = 0; i < 50; i++)
			{
				this->DepthJobList[i].DepthMap = std::make_shared<GDepthMap>();
				this->DepthJobList[i].DepthMap->AllocateDescriptor();
			}*/
			Test.Camera = std::make_shared<GOrthoCamera>();
			Test.CameraCBuffer = GCameraCBuffer::CreateNewCameraCBuffer(
				GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::DEPTH_PSO)->GetPipelineRootSignature());
			Test.DepthMap = std::make_shared<GDepthMap>(1024, 1024);
			Test.DepthMap->SetRootParameterIndex(GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature()->
				GetRootParameterIndex(GRootSignature::RootParameter(GRootSignature::GE_PARAMETER_SRV, 5)));
			Test.DepthMap->SetShaderViewName("Light_1");
			if (!GShaderViewRegistry::HasShaderView("Light_1"))
			{
				GShaderViewRegistry::RegistryShaderView(Test.DepthMap);
			}

			this->AddApplicable(Test.CameraCBuffer);
		}

		void Apply() override
		{
			Test.DepthMap->ApplyDepthMap();
			Test.DepthMap->ClearDepthMap();

			for (auto& applicable : this->ApplicableList)
			{
				applicable->Apply();
			}
		}

		void Execute() override
		{
			GMatrix CameraMatrix = Test.Camera->GetViewMatrix() * Test.Camera->Projection.GetProjectionMatrix();
			CameraMatrix.Transpose();
			Test.CameraCBuffer->UpdateBufferData({ CameraMatrix, Test.Camera->Position });

			this->UpdateLightData();

			Test.DepthMap->BeginRendering();
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->ApplyDescriptorHeaps();

			this->Apply();

			for (auto& task : this->TaskList)
			{
				task->Execute();
			}

			Test.DepthMap->EndUpRendering();
		}

		void Finalize() override
		{

		}

	private:
		void UpdateLightData();

		struct DepthJob
		{
			std::shared_ptr<GCameraCBuffer> CameraCBuffer;
			std::shared_ptr<GOrthoCamera> Camera;
			
			std::shared_ptr<GDepthMap> DepthMap;
		};

		std::vector<DepthJob> DepthJobList;
		DepthJob Test;
	};
}

#endif