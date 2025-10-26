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
	}
}
