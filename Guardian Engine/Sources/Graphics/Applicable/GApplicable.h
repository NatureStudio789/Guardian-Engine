#ifndef _GE_GAPPLICABLE_H_
#define _GE_GAPPLICABLE_H_
#include "../GGraphicsContextRegistry.h"

namespace GE
{
	class GUARDIAN_API GApplicable
	{
	public:
		GApplicable() = default;
		GApplicable(const GApplicable&) = default;
		virtual ~GApplicable() = default;

		virtual void Apply() = 0;
	};
}

#endif