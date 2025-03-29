#ifndef _GE_GUARDIANTIMEPOINT_H_
#define _GE_GUARDIANTIMEPOINT_H_
#include "../../Serializable/GuardianSerializable.h"

namespace GE
{
	class GUARDIAN_API GuardianTimepoint
	{
	public:
		GuardianTimepoint();
		GuardianTimepoint(float timePoint);
		GuardianTimepoint(const GuardianTimepoint& other);
		~GuardianTimepoint() = default;

		const float operator-(const GuardianTimepoint& other) const;
		const float operator-(const float timePoint) const;

		const float GetMillisecond() const noexcept;
		const float GetSecond() const noexcept;

		static const GuardianTimepoint GetCurrentTimepoint() noexcept;

	private:
		float Timepoint;
	};
}

#endif