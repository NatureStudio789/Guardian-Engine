#ifndef _GE_GRENDERENGINE_H_
#define _GE_GRENDERENGINE_H_
#include "../Audio/GAudioEngine.h"

namespace GE
{
	class GUARDIAN_API GRenderEngine : public GModule
	{
	public:
		GRenderEngine();
		GRenderEngine(const GRenderEngine& other);
		~GRenderEngine() override;

		void InitializeModule() override;
		void UpdateModule() override;
		void ReleaseModule() override;

	private:
		std::shared_ptr<GGraphicsContext> MainGraphicsContext;
		std::shared_ptr<GModel> TestM;
	};
}

#endif