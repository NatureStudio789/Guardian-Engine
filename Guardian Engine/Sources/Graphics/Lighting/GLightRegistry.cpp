#include "GLightRegistry.h"

namespace GE
{
	void GLightRegistry::SubmitPointLight(const GPointLight& pointLight)
	{
		this->PointLightList.push_back(pointLight);

		if (this->PointLightList.size() > GLightRegistry::MaxLightCount)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The point light count is larger than which the engine supports");
		}
	}

	void GLightRegistry::Update()
	{
		this->PointLightList.clear();
	}

	bool GLightRegistry::HasPointLights()
	{
		return this->PointLightList.empty() != true;
	}

	const std::vector<GPointLight>& GLightRegistry::GetPointLightList()
	{
		return this->PointLightList;
	}
}
