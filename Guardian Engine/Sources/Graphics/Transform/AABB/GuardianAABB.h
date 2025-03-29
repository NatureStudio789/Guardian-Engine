#ifndef _GE_GUARDIANAABB_H_
#define _GE_GUARDIANAABB_H_
#include "../Vector/GuardianVector.h"
#include <cmath>

namespace GE
{
	struct GUARDIAN_API GuardianAABB
	{
		GuardianAABB()
		{
			this->Min = { 0.0f };
			this->Max = { 0.0f };
		}

		GuardianAABB(const GVector3& min, const GVector3& max)
		{
			this->Min = min;
			this->Max = max;
		}

		GuardianAABB(const GuardianAABB& other)
		{
			this->Min = other.Min;
			this->Max = other.Max;
		}

		const bool Intersect(const GVector3& rayOrigin, const GVector3& rayDirection, float& t) const
		{
			GVector3 dirfrac;
			dirfrac.x = 1.0f / rayOrigin.x;
			dirfrac.y = 1.0f / rayOrigin.y;
			dirfrac.z = 1.0f / rayOrigin.z;

			const GVector3& lb = this->Min;
			const GVector3& rt = this->Max;
			float t1 = (lb.x - rayOrigin.x) * dirfrac.x;
			float t2 = (rt.x - rayOrigin.x) * dirfrac.x;
			float t3 = (lb.y - rayOrigin.y) * dirfrac.y;
			float t4 = (rt.y - rayOrigin.y) * dirfrac.y;
			float t5 = (lb.z - rayOrigin.z) * dirfrac.z;
			float t6 = (rt.z - rayOrigin.z) * dirfrac.z;

			float tmin = max(max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
			float tmax = std::min(std::min(max(t1, t2), max(t3, t4)), max(t5, t6));

			if (tmax < 0)
			{
				t = tmax;
				return false;
			}

			if (tmin > tmax)
			{
				t = tmax;
				return false;
			}

			t = tmin;
			return true;
		}

		GVector3 Min, Max;
	};
}

#endif