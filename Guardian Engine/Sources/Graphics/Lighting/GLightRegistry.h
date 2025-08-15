#ifndef _GE_GLIGHTREGISTRY_H_
#define _GE_GLIGHTREGISTRY_H_
#include "PointLight/GPointLight.h"

namespace GE
{
	class GUARDIAN_API GLightRegistry
	{
	public:
		GLightRegistry() = default;
		GLightRegistry(const GLightRegistry&) = default;

		void SubmitPointLight(const GPointLight& pointLight);

		void Update();

		bool HasPointLights();
		const std::vector<GPointLight>& GetPointLightList();

		static constexpr UINT MaxLightCount = 50;

	private:
		std::vector<GPointLight> PointLightList;
	};
}

#endif