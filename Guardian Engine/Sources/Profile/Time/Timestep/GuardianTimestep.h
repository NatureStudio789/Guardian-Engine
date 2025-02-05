#ifndef _GE_GUARDIANTIMESTEP_H_
#define _GE_GUARDIANTIMESTEP_H_
#include "../Timepoint/GuardianTimepoint.h"

namespace guardian
{
	class GUARDIAN_API GuardianTimestep
	{
	public:
		GuardianTimestep();
		GuardianTimestep(const float timeDistance);
		GuardianTimestep(const GuardianTimestep& other);
		~GuardianTimestep() = default;

		const GuardianTimestep operator+(const GuardianTimestep& other) const;
		const GuardianTimestep operator+(const float timeDistance) const;
		const GuardianTimestep operator-(const GuardianTimestep& other) const;
		const GuardianTimestep operator-(const float timeDistance) const;

		GuardianTimestep& operator+=(const GuardianTimestep& other);
		GuardianTimestep& operator+=(const float timeDistance);
		GuardianTimestep& operator-=(const GuardianTimestep& other);
		GuardianTimestep& operator-=(const float timeDistance);

		operator float() const;

		const float GetSecond() const noexcept;
		const float GetMillisecond() const noexcept;

	private:
		float TimeDistance;
	};
}

#endif