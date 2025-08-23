#ifndef _GE_GPROJECTENGINE_H_
#define _GE_GPROJECTENGINE_H_
#include "../Graphics/GRenderEngine.h"

namespace GE
{
	class GUARDIAN_API GProjectEngine : public GModule
	{
	public:
		GProjectEngine();
		GProjectEngine(const GProjectEngine& other);
		~GProjectEngine() override;

		void InitializeModule() override;
		void UpdateModule() override;
		void ReleaseModule() override;

	private:

	};
}

#endif