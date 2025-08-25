#include "GAudioContext.h"

namespace GE
{
    GAudioContext::GAudioContext()
    {
        this->ContextId = 0;
        this->ContextName = "";
    }

    GAudioContext::GAudioContext(const std::string& name)
    {
        this->InitializeAudioContext(name);
    }

    GAudioContext::GAudioContext(const GAudioContext& other)
    {
        this->ContextId = other.ContextId;
        this->ContextName = other.ContextName;

        this->AudioEngine = other.AudioEngine;
    }

    GAudioContext::~GAudioContext()
    {
        this->ContextId = 0;
        this->ContextName.clear();

        this->AudioEngine.reset();
    }

    void GAudioContext::InitializeAudioContext(const std::string& name)
    {
        this->ContextId = GUUID();
        this->ContextName = name;

        this->AudioEngine = std::make_shared<DirectX::AudioEngine>(DirectX::AudioEngine_Default);
    }

    void GAudioContext::Update()
    {
        if (!this->AudioEngine->Update())
        {
            throw GUARDIAN_ERROR_EXCEPTION("Failed to update audio engine!");
        }
    }
    
    const GUUID& GAudioContext::GetContextId() const noexcept
    {
        return this->ContextId;
    }

    const std::string& GAudioContext::GetContextName() const noexcept
    {
        return this->ContextName;
    }

    std::shared_ptr<DirectX::AudioEngine> GAudioContext::GetAudioEngine()
    {
        return this->AudioEngine;
    }
}
