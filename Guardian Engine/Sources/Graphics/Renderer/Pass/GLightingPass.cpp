#include "GLightingPass.h"
#include "../../../Gameplay/Scene/GSceneRegistry.h"

namespace GE
{
	void GLightingPass::UpdateLightCBuffer()
	{
		GLightCBData LightData;
		const auto& PointLightList = GSceneRegistry::GetActiveScene()->GetLightRegistry()->GetPointLightList();
		LightData.PointLightCount = (int)PointLightList.size();
		for (UINT i = 0; i < (UINT)PointLightList.size(); i++)
		{
			LightData.PointLightList[i] = PointLightList[i];
		}
		this->LightCBuffer->UpdateBufferData(LightData);
	}
}
