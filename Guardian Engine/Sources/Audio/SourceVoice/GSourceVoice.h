#ifndef _GE_GSOURCEVOICE_H_
#define _GE_GSOURCEVOICE_H_
#include "../GAudioContextRegistry.h"

namespace GE
{
	class GUARDIAN_API GSourceVoice
	{
	public:
		GSourceVoice();
		GSourceVoice(const std::string& filePath);
		GSourceVoice(const GSourceVoice& other);
		~GSourceVoice();

		void InitializeSourceVoice(const std::string& filePath);

		void PlaySourceVoice();

		IXAudio2SourceVoice* GetSourceVoiceObject();

		static std::shared_ptr<GSourceVoice> CreateNewSourceVoice(
			const std::string& filePath)
		{
			return std::make_shared<GSourceVoice>(filePath);
		}

	private:
		IXAudio2SourceVoice* SourceVoiceObject;
		FILE* VoiceFile;
		XAUDIO2_BUFFER AudioBuffer[4];
		BYTE* VoiceFileBuffer[4];
		bool IsVoicePlaying;
	};
}

#endif