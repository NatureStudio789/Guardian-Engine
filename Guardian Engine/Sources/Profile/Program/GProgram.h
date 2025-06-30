#ifndef _GE_GPROGRAM_H_
#define _GE_GPROGRAM_H_
#include "../Core/GCore.h"

namespace GE
{
	class GUARDIAN_API GProgram
	{
	public:
		struct Attribute
		{
		public:
			std::string ProgramName;
		};

	public:
		GProgram() {}
		virtual ~GProgram() { this->ReleaseProgram(); }

		virtual void PreInitializeProgram()
		{
			ProgramAttribute.ProgramName = "GuardianProgram";
		}
		virtual void InitializeProgram() {}
		virtual void UpdateProgram() {}
		virtual void ReleaseProgram() {}

		const Attribute& GetProgramAttribute() const noexcept
		{
			return this->ProgramAttribute;
		}
		const std::string& GetProgramName() const noexcept
		{
			return this->ProgramAttribute.ProgramName;
		}

	protected:
		Attribute ProgramAttribute;
	};
}

#endif