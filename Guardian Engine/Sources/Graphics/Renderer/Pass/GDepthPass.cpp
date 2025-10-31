#include "GDepthPass.h"
#include "../../../Gameplay/Scene/GSceneRegistry.h"

namespace GE
{
	void GDepthPass::UpdateLightData()
	{
		if (GSceneRegistry::GetActiveScene()->GetLightRegistry()->HasPointLights())
		{
			auto& firstLight = GSceneRegistry::GetActiveScene()->GetLightRegistry()->GetPointLightList().front();
			Test.Camera->Position = firstLight.Position;
		}

		if (GetAsyncKeyState('E'))
		{
			Test.Camera->Rotate({ 0.0f, 0.1f, 0.0f });
		}
		if (GetAsyncKeyState('Q'))
		{
			Test.Camera->Rotate({ 0.0f, -0.1f, 0.0f });
		}
	}
}
