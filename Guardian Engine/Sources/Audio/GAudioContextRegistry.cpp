#include "GAudioContextRegistry.h"

namespace GE
{
	std::string GAudioContextRegistry::CurrentAudioContext;
	std::map<std::string, std::shared_ptr<GAudioContext>> GAudioContextRegistry::AudioContextList;


	void GAudioContextRegistry::RegisterAudioContext(std::shared_ptr<GAudioContext> context)
	{
		if (AudioContextList.count(context->GetContextName()) > 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The Audio context named '{}' has been in registry.", context->GetContextName()));
		}

		AudioContextList[context->GetContextName()] = context;
	}

	void GAudioContextRegistry::SetCurrentAudioContext(const std::string& name)
	{
		if (AudioContextList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No Audio context named '{}' found.", name));
		}

		CurrentAudioContext = name;
	}

	void GAudioContextRegistry::SetCurrentAudioContext(const GUUID& id)
	{
		for (auto& context : AudioContextList)
		{
			if (context.second->GetContextId() == id)
			{
				CurrentAudioContext = context.first;
				return;
				break;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No Audio context with id '{}' found.", (unsigned long long)id));
	}

	std::shared_ptr<GAudioContext> GAudioContextRegistry::GetCurrentAudioContext()
	{
		if (AudioContextList.count(CurrentAudioContext) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("Current Audio context named '{}' is invalid.", CurrentAudioContext));
		}

		return AudioContextList[CurrentAudioContext];
	}

	std::shared_ptr<GAudioContext> GAudioContextRegistry::GetAudioContext(const std::string& name)
	{
		if (AudioContextList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No Audio context named '{}' found.", name));
		}

		return AudioContextList[name];
	}

	std::shared_ptr<GAudioContext> GAudioContextRegistry::GetAudioContext(const GUUID& id)
	{
		for (auto& context : AudioContextList)
		{
			if (context.second->GetContextId() == id)
			{
				return context.second;
				break;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No Audio context with id '{}' found.", (unsigned long long)id));
	}
}
