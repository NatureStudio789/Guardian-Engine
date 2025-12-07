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
			pointLight->LightDepthCubeMap->BeginRendering();
			
			for (UINT i = 0; i < 6; i++)
			{
				pointLight->UpdateDepthRendering(i);

				GGraphicsContextRegistry::GetCurrentGraphicsContext()->ApplyDescriptorHeaps();

				pointLight->LightDepthCubeMap->ApplyDepthCubeMap(i);
				pointLight->LightDepthCubeMap->ClearDepthCubeMap(i);

				this->Apply();
				pointLight->ApplyDepthRendering(i);

				for (auto& task : this->TaskList)
				{
					task->Execute();
				}

			}

			pointLight->LightDepthCubeMap->EndUpRendering();
		}
	}
}
