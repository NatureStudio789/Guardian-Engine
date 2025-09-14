#ifndef _GE_GAABB_H_
#define _GE_GAABB_H_
#include "../Matrix/GMatrix.h"

namespace GE
{
	class GUARDIAN_API GAABB
	{
	public:
		GAABB();
		GAABB(const GVector3& max, const GVector3& min);
		GAABB(const GAABB& other);

		void SetTransform(GMatrix transformMatrix);
		bool Intersect(const GVector3& rayOrigin, const GVector3& rayDirection, float* distance = null) const;

		GVector3 Min;
		GVector3 Max;
	};
}

#endif