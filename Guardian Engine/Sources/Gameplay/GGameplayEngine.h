#ifndef _GE_GGAMEPLAYENGINE_H_
#define _GE_GGAMEPLAYENGINE_H_
#include "../Graphics/GRenderEngine.h"

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
		std::shared_ptr<GScene> ActiveScene;
		std::shared_ptr<GEntity> Nanosuit;
		std::shared_ptr<GEntity> Hasake;
	};
}

#endif