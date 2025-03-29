#ifndef _GE_GUARDIANPOINTLIGHT_H_
#define _GE_GUARDIANPOINTLIGHT_H_
#include "../../GuardianGraphics.h"

namespace GE
{
	struct GUARDIAN_API GuardianPointLightProperties
	{
	public:
		GuardianPointLightProperties()
		{
			this->LightPosition = GVector3(0.0f, 0.0f, 0.0f);
			this->LightColor = GVector3(1.0f, 1.0f, 1.0f);
			this->LightStrength = 100.0f;
		}
		GuardianPointLightProperties(const GVector3& lightPosition,
			const GVector3& lightColor, const float lightStrength)
		{
			this->LightPosition = lightPosition;
			this->LightColor = lightColor;
			this->LightStrength = lightStrength;
		}

		GVector3 LightPosition;
		float LightStrength;
		GVector3 LightColor;
	private:
		float padding;
	};
}

#endif