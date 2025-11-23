#ifndef _GE_GSCRIPTENGINE_H_
#define _GE_GSCRIPTENGINE_H_
#include "../Asset/GAssetEngine.h"

namespace GE
{
	class GUARDIAN_API GScriptEngine : public GModule
	{
	public:
		GScriptEngine();
		GScriptEngine(const GScriptEngine& other);
		~GScriptEngine();

		void InitializeModule() override;
		void UpdateModule() override;
		void ReleaseModule() override;

	private:

	};
}

#endif