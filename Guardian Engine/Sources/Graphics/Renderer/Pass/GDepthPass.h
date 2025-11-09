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
			/*Test.Camera = std::make_shared<GOrthoCamera>();
			Test.CameraCBuffer = GCameraCBuffer::CreateNewCameraCBuffer(
				GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::DEPTH_PSO)->GetPipelineRootSignature());
			Test.DepthMap = std::make_shared<GDepthMap>(1024, 1024);
			Test.DepthMap->SetRootParameterIndex(GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature()->
				GetRootParameterIndex(GRootSignature::RootParameter(GRootSignature::GE_PARAMETER_SRV, 5, 50)));
			Test.DepthMap->SetShaderViewName("Light_1");
			if (!GShaderViewRegistry::HasShaderView("Light_1"))
			{
				GShaderViewRegistry::RegistryShaderView(Test.DepthMap);
			}

			this->AddApplicable(Test.CameraCBuffer);*/
		}

		void Apply() override
		{
			for (auto& applicable : this->ApplicableList)
			{
				applicable->Apply();
			}
		}

		void Execute() override;

		void Finalize() override {}

	private:
	};
}

#endif