#ifndef _GE_GUARDIANLIGHTSYSTEM_H_
#define _GE_GUARDIANLIGHTSYSTEM_H_
#include "PointLight/GuardianPointLight.h"

namespace GE
{
	class GUARDIAN_API GuardianLightSystem
	{
	public:
		static void SubmitPointLight(const GuardianPointLightProperties& light);

		static bool IsPointLightListEmpty();
		static const GuardianPointLightProperties ReadPointLight();

	private:
		static std::queue<GuardianPointLightProperties> PointLightList;
	};
}

#endif