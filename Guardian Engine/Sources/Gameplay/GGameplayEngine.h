#ifndef _GE_GGAMEPLAYENGINE_H_
#define _GE_GGAMEPLAYENGINE_H_
#include "../Asset/GAssetEngine.h"

namespace GE
{
	class GUARDIAN_API GGameplayEngine : public GModule
	{
	public:
		GGameplayEngine();
		GGameplayEngine(const GGameplayEngine& other);
		~GGameplayEngine() override;

		void InitializeModule() override;
		void UpdateModule() override;
		void ReleaseModule() override;

	private:
	};
}

#endif