#include "GAudioContext.h"

namespace GE
{
    GAudioContext::GAudioContext()
    {
        this->ContextId = GUUID();
    }

    GAudioContext::GAudioContext(const GAudioContext& other)
    {
        this->ContextId = other.ContextId;
        this->AudioObject = other.AudioObject;
        this->MasteringVoice = other.MasteringVoice;
    }

    GAudioContext::~GAudioContext()
    {
        this->ContextId = 0;
    }

    void GAudioContext::InitializeAudioContext()
    {
        GUARDIAN_SETUP_AUTO_THROW();

        GUARDIAN_AUTO_THROW(CoInitializeEx(null, COINIT_MULTITHREADED));

        GUARDIAN_AUTO_THROW(XAudio2Create(
            &this->AudioObject, 0, XAUDIO2_DEFAULT_PROCESSOR));

        GUARDIAN_AUTO_THROW(this->AudioObject->CreateMasteringVoice(
            &this->MasteringVoice));
    }
    
    const GUUID& GAudioContext::GetContextId() const noexcept
    {
        return this->ContextId;
    }

    IXAudio2* GAudioContext::GetAudioObject()
    {
        return this->AudioObject;
    }

    IXAudio2MasteringVoice* GAudioContext::GetMasteringVoice()
    {
        return this->MasteringVoice;
    }
}
