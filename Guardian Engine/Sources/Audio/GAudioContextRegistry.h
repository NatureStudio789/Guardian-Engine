#ifndef _GE_GAUDIOCONTEXTREGISTRY_H_
#define _GE_GAUDIOCONTEXTREGISTRY_H_
#include "GAudioContext.h"

namespace GE
{
	class GUARDIAN_API GAudioContextRegistry
	{
	public:
		static void RegisterAudioContext(std::shared_ptr<GAudioContext> context);

		static void SetCurrentAudioContext(const std::string& name);
		static void SetCurrentAudioContext(const GUUID& id);

		static std::shared_ptr<GAudioContext> GetCurrentAudioContext();

		static std::shared_ptr<GAudioContext> GetAudioContext(const std::string& name);
		static std::shared_ptr<GAudioContext> GetAudioContext(const GUUID& id);

	private:
		static std::string CurrentAudioContext;
		static std::map<std::string, std::shared_ptr<GAudioContext>> AudioContextList;
	};
}

#endif