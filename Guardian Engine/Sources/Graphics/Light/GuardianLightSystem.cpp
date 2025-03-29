#include "GuardianLightSystem.h"

namespace GE
{
	std::queue<GuardianPointLightProperties> GuardianLightSystem::PointLightList;


	void GuardianLightSystem::SubmitPointLight(const GuardianPointLightProperties& light)
	{
		PointLightList.push(light);
	}

	bool GuardianLightSystem::IsPointLightListEmpty()
	{
		return PointLightList.empty();
	}

	const GuardianPointLightProperties GuardianLightSystem::ReadPointLight()
	{
		if (!IsPointLightListEmpty())
		{
			GuardianPointLightProperties pointLight = PointLightList.front();
			PointLightList.pop();

			return pointLight;
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION("Cannot read the empty point light list!");
		}
	}
}
