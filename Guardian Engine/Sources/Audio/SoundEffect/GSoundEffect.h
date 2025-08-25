#ifndef _GE_GSOUNDEFFECT_H_
#define _GE_GSOUNDEFFECT_H_
#include "../GAudioContextRegistry.h"

namespace GE
{
	class GUARDIAN_API GSoundEffect
	{
	public:
		GSoundEffect();
		GSoundEffect(const std::string& filePath);
		GSoundEffect(const GSoundEffect& other);
		~GSoundEffect();

		void InitializeSoundEffect(const std::string& filePath);

		void Play();

		static std::shared_ptr<GSoundEffect> CreateNewSoundEffect(const std::string& filePath)
		{
			return std::make_shared<GSoundEffect>(filePath);
		}

	private:
		std::shared_ptr<DirectX::SoundEffect> SoundEffectObject;
		std::unique_ptr<DirectX::SoundEffectInstance> SoundEffectInstance;
	};
}

#endif