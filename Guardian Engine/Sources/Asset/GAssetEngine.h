#ifndef _GE_GASSETENGINE_H_
#define _GE_GASSETENGINE_H_
#include "../Graphics/GRenderEngine.h"

namespace GE
{
	class GUARDIAN_API GAssetEngine : public GModule
	{
	public:
		GAssetEngine();
		GAssetEngine(const GAssetEngine& other);
		~GAssetEngine() override;

		void InitializeModule() override;
		void UpdateModule() override;
		void ReleaseModule() override;

	private:

	};
}

#endif