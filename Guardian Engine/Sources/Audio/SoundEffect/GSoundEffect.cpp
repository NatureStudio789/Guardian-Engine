#include "GSoundEffect.h"

namespace GE
{
	GSoundEffect::GSoundEffect()
	{
		this->SoundEffectObject = null;
		this->SoundEffectInstance = null;
	}

	GSoundEffect::GSoundEffect(const std::string& filePath)
	{
		this->InitializeSoundEffect(filePath);
	}

	GSoundEffect::GSoundEffect(const GSoundEffect& other)
	{
		this->SoundEffectObject = other.SoundEffectObject;
	}

	GSoundEffect::~GSoundEffect()
	{
		this->SoundEffectObject.reset();
		this->SoundEffectObject = null;

		this->SoundEffectInstance.reset();
		this->SoundEffectInstance = null;
	}

	void GSoundEffect::InitializeSoundEffect(const std::string& filePath)
	{
		this->SoundEffectObject = std::make_shared<DirectX::SoundEffect>(
			GAudioContextRegistry::GetCurrentAudioContext()->GetAudioEngine().get(), GUtil::StringToWideString(filePath).c_str());
	}

	void GSoundEffect::Play()
	{
		if (!this->SoundEffectInstance)
		{
			this->SoundEffectInstance = this->SoundEffectObject->CreateInstance();
		}

		this->SoundEffectInstance->Play();
	}
}
