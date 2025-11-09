#ifndef _GE_GLIGHTREGISTRY_H_
#define _GE_GLIGHTREGISTRY_H_
#include "PointLight/GPointLight.h"

namespace GE
{
	class GUARDIAN_API GShaderViewGroup;

	class GUARDIAN_API GLightRegistry
	{
	public:
		GLightRegistry();
		GLightRegistry(const GLightRegistry& other);

		void RegisterPointLight(GPointLight* pointLight);
		void UnregisterPointLight(GPointLight* pointLight);
		void UnregisterPointLight(GUUID id);

		bool HasPointLight(GPointLight* pointLight);
		bool HasPointLight(GUUID id);

		bool HasPointLights();
		std::vector<GPointLight*> GetPointLightList();

		static constexpr UINT MaxLightCount = 50;

	private:
		std::vector<GPointLight*> PointLightList;

		std::shared_ptr<GShaderViewGroup> PointLightDepthMapGroup;
	};
}

#endif