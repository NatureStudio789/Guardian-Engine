#include "GScriptEngine.h"

namespace GE
{
	GScriptEngine::GScriptEngine()
	{

	}

	GScriptEngine::GScriptEngine(const GScriptEngine& other)
	{

	}

	GScriptEngine::~GScriptEngine()
	{

	}

	void GScriptEngine::InitializeModule()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* RootDomain = mono_jit_init("GuardianJITRuntime");
		if (!RootDomain)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to initialize script root domain!");
		}
	}

	void GScriptEngine::UpdateModule()
	{

	}

	void GScriptEngine::ReleaseModule()
	{

	}
}
