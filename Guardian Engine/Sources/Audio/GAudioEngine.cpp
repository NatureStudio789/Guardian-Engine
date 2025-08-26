#include "GAudioEngine.h"

namespace GE
{
	GAudioEngine::GAudioEngine()
	{
		this->MainAudioContext = null;

		this->ModuleName = "Audio";
	}

	GAudioEngine::GAudioEngine(const GAudioEngine& other)
	{
		
	}

	GAudioEngine::~GAudioEngine()
	{

	}

	void GAudioEngine::InitializeModule()
	{
		this->MainAudioContext = GAudioContext::CreateNewContext("Main");

		GAudioContextRegistry::RegisterAudioContext(this->MainAudioContext);
		GAudioContextRegistry::SetCurrentAudioContext("Main");

		/*Test = GSoundEffect::CreateNewSoundEffect("Assets\\Audios\\I'm Alive.wav");
		Test->Play();*/
	}

	void GAudioEngine::UpdateModule()
	{
		GAudioContextRegistry::GetCurrentAudioContext()->Update();
	}

	void GAudioEngine::ReleaseModule()
	{

	}
}
