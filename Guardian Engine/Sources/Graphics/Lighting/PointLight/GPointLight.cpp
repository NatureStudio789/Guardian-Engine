#include "GPointLight.h"
#include "../../../Gameplay/Scene/GScene.h"

namespace GE
{
	GPointLight::GPointLight()
	{
		this->Position = {};
		this->Strength = 100.0f;
		this->Color = { 1.0f, 1.0f, 1.0f };
	}

	GPointLight::GPointLight(const GVector3& position,
		const GVector3& color, float strength)
	{
		this->Position = position;
		this->Color = color;
		this->Strength = strength;
	}

	void GPointLight::Submit(GScene* scene)
	{
		scene->GetLightRegistry()->SubmitPointLight(*this);
	}
}
