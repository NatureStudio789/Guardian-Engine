#ifndef _GE_GUARDIANAPPLICABLE_H_
#define _GE_GUARDIANAPPLICABLE_H_
#include "../Light/GuardianLightSystem.h"

namespace GE
{
	class GUARDIAN_API GuardianApplicable
	{
	public:
		GuardianApplicable() = default;
		GuardianApplicable(const GuardianApplicable&) = default;
		virtual ~GuardianApplicable() = default;

		virtual void Apply() = 0;
	};
}

#endif