#include "GuardianProgram.h"

namespace GE
{
	void GuardianProgram::LaunchProgram()
	{
		if (this->ProgramMode == GE_NONE_MODE)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Invalid mode for program launching!");
		}

		this->IsProgramExecuted = true;
	}

	void GuardianProgram::ShutdownProgram()
	{
		this->ProgramMode = GE_NONE_MODE;
		this->IsProgramExecuted = false;
	}

	bool GuardianProgram::IsProgramRunning() const noexcept
	{
		return this->IsProgramExecuted;
	}

	void GuardianProgram::SetProgramMode(const Mode& mode)
	{
		this->ProgramMode = mode;
	}

	const GuardianProgram::Mode& GuardianProgram::GetProgramMode(const Mode& mode) const noexcept
	{
		return this->ProgramMode;
	}

	const GString& GuardianProgram::GetProgramName() const noexcept
	{
		return this->ProgramName;
	}
}
