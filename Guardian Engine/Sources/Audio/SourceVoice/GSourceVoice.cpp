#include "GSourceVoice.h"

namespace GE
{
	GSourceVoice::GSourceVoice()
	{
		this->IsVoicePlaying = false;
	}

	GSourceVoice::GSourceVoice(const std::string& filePath)
	{
		this->InitializeSourceVoice(filePath);
	}

	GSourceVoice::GSourceVoice(const GSourceVoice& other)
	{
		this->SourceVoiceObject = other.SourceVoiceObject;
		this->IsVoicePlaying = other.IsVoicePlaying;
	}

	GSourceVoice::~GSourceVoice()
	{
		this->IsVoicePlaying = false;

		for (int i = 0; i < 4; i++)
		{
			if (VoiceFileBuffer[i])
			{
				free(VoiceFileBuffer[i]);
			}
		}
	}

	void GSourceVoice::InitializeSourceVoice(const std::string& filePath)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		WAVEFORMATEX Format;
		GUARDIAN_CLEAR_MEMORY(Format);
		Format.wFormatTag = WAVE_FORMAT_PCM;
		Format.nChannels = 2;
		Format.wBitsPerSample = 16;
		Format.nSamplesPerSec = 44100;
		Format.nBlockAlign = Format.nChannels * Format.wBitsPerSample / 8;
		Format.nAvgBytesPerSec = Format.nSamplesPerSec * Format.nBlockAlign;
		Format.cbSize = 0;

		GUARDIAN_AUTO_THROW(GAudioContextRegistry::GetCurrentAudioContext()->GetAudioObject()->
			CreateSourceVoice(&SourceVoiceObject, &Format));

		fopen_s(&VoiceFile, filePath.c_str(), "rb");
		
		for (UINT i = 0; i < 4; i++)
		{
			VoiceFileBuffer[i] = (BYTE*)malloc(640);
			if (!VoiceFileBuffer[i])
			{
				throw GUARDIAN_ERROR_EXCEPTION("Failed to malloc memory for audio file buffer");
			}
			fread(VoiceFileBuffer[i], 640, 1, VoiceFile);
			GUARDIAN_CLEAR_MEMORY(AudioBuffer[i]);
			AudioBuffer[i].Flags = 0;
			AudioBuffer[i].pAudioData = VoiceFileBuffer[i];
			AudioBuffer[i].AudioBytes = 640;
			AudioBuffer[i].LoopBegin = 0;
			AudioBuffer[i].LoopCount = 0;
			AudioBuffer[i].LoopLength = 0;
			AudioBuffer[i].pContext = (void*)new int[1];
			AudioBuffer[i].PlayBegin = 0;
			AudioBuffer[i].PlayLength = 0;

			GUARDIAN_AUTO_THROW(this->SourceVoiceObject->SubmitSourceBuffer(&AudioBuffer[i]));
		}
	}

	void GSourceVoice::PlaySourceVoice()
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->IsVoicePlaying = true;
		GUARDIAN_AUTO_THROW(this->SourceVoiceObject->Start());

		UINT Index = 0;
		while (this->IsVoicePlaying)
		{
			XAUDIO2_VOICE_STATE VoiceState;
			this->SourceVoiceObject->GetState(&VoiceState);
			this->IsVoicePlaying = (VoiceState.BuffersQueued > 0);
			if (VoiceState.BuffersQueued <= 2)
			{
				if (!fread(this->VoiceFileBuffer[Index], 640, 1, this->VoiceFile))
				{
					break;
				}

				this->AudioBuffer[Index].pAudioData = this->VoiceFileBuffer[Index];
				GUARDIAN_AUTO_THROW(this->SourceVoiceObject->SubmitSourceBuffer(
					&this->AudioBuffer[Index]));

				Index = (Index + 1) % 4;
			}
		}

		GUARDIAN_AUTO_THROW(this->SourceVoiceObject->Stop());
	}

	IXAudio2SourceVoice* GSourceVoice::GetSourceVoiceObject()
	{
		return this->SourceVoiceObject;
	}
}
