#include "GDepthPass.h"
#include "../../../Gameplay/Scene/GSceneRegistry.h"

namespace GE
{
	void GDepthPass::Execute()
	{
		/*GMatrix CameraMatrix = Test.Camera->GetViewMatrix() * Test.Camera->Projection.GetProjectionMatrix();
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

		Test.DepthMap->EndUpRendering();*/

		for (auto& pointLight : GSceneRegistry::GetActiveScene()->GetLightRegistry()->GetPointLightList())
		{
			pointLight->UpdateDepthRendering();

			pointLight->LightDepthMap->BeginRendering();
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->ApplyDescriptorHeaps();

			pointLight->LightDepthMap->ApplyDepthMap();
			pointLight->LightDepthMap->ClearDepthMap();

			this->Apply();
			pointLight->LightCameraCBuffer->Apply();

			for (auto& task : this->TaskList)
			{
				task->Execute();
			}

			pointLight->LightDepthMap->EndUpRendering();
		}
	}
}
