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
		this->MainAudioContext = GAudioContext::CreateNewContext();

		GAudioContextRegistry::RegisterAudioContext("Main", this->MainAudioContext);
		GAudioContextRegistry::SetCurrentAudioContext("Main");

		this->TestVoice = GSourceVoice::CreateNewSourceVoice("C:\\Users\\Administrator\\Downloads\\At The Edge - วงÛเ.wav");
	}

	void GAudioEngine::UpdateModule()
	{
		this->TestVoice->PlaySourceVoice();
	}

	void GAudioEngine::ReleaseModule()
	{

	}
}
