#ifndef _GE_GAUDIOCONTEXT_H_
#define _GE_GAUDIOCONTEXT_H_
#include "../Aplication/Event/GMouseEvent.h"

namespace GE
{
	class GAudioContext
	{
	public:
		GAudioContext();
		GAudioContext(const GAudioContext& other);
		~GAudioContext();

		void InitializeAudioContext();

		const GUUID& GetContextId() const noexcept;
		IXAudio2* GetAudioObject();
		IXAudio2MasteringVoice* GetMasteringVoice();

		static std::shared_ptr<GAudioContext> CreateNewContext()
		{
			auto context = std::make_shared<GAudioContext>();
			context->InitializeAudioContext();
			return context;
		}

	private:
		GUUID ContextId;
		IXAudio2* AudioObject;
		IXAudio2MasteringVoice* MasteringVoice;
	};
}

#endif