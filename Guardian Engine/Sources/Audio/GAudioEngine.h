#ifndef _GE_GAUDIOENGINE_H_
#define _GE_GAUDIOENGINE_H_
#include "../Profile/Module/GModule.h"

namespace GE
{
	class GUARDIAN_API GAudioEngine : public GModule
	{
	public:
		GAudioEngine();
		GAudioEngine(const GAudioEngine& other);
		~GAudioEngine() override;

		void InitializeModule() override;
		void UpdateModule() override;
		void ReleaseModule() override;

	private:
		std::shared_ptr<GAudioContext> MainAudioContext;
		std::shared_ptr<GSoundEffect> Test;
	};
}

#endif