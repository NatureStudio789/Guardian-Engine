#ifndef _GE_GAPPLICABLE_H_
#define _GE_GAPPLICABLE_H_
#include "../DepthMap/GDepthMap.h"

namespace GE
{
	class GUARDIAN_API GRenderable;

	class GUARDIAN_API GApplicable
	{
	public:
		GApplicable() = default;
		GApplicable(const GApplicable&) = default;
		virtual ~GApplicable() = default;
		
		virtual void SetParent(const GRenderable&) {}
		virtual void Apply() {}
	};
}

#endif