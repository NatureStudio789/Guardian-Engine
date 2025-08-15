#ifndef _GE_GPOINTLIGHT_H_
#define _GE_GPOINTLIGHT_H_
#include "../../Math/Camera/GCamera.h"

namespace GE
{
	class GUARDIAN_API GScene;

	struct GUARDIAN_API GPointLight
	{
	public:
		GPointLight();
		GPointLight(const GPointLight&) = default;
		GPointLight(const GVector3& position,
			const GVector3& color, float strength);

		void Submit(GScene* scene);

		GVector3 Position;
		float Strength;
		GVector3 Color;
	private:
		float padding1;
	};
}

#endif