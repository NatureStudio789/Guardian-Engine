#pragma once
#include <Profile/EntryPoint/GuardianEntryPoint.h>

class Sandbox : public guardian::GuardianProgram
{
public:
	void Initialize() override;
	void Update() override;
};

GUARDIAN_ENGINE_PROFILE(Sandbox)
