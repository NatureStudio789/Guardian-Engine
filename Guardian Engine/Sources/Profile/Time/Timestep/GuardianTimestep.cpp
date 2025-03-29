#include "GuardianTimestep.h"

namespace GE
{
	GuardianTimestep::GuardianTimestep()
	{
		this->TimeDistance = 0.0f;
	}

	GuardianTimestep::GuardianTimestep(const float timeDistance)
	{
		this->TimeDistance = timeDistance;
	}

	GuardianTimestep::GuardianTimestep(const GuardianTimestep& other)
	{
		this->TimeDistance = other.TimeDistance;
	}

	const GuardianTimestep GuardianTimestep::operator+(const GuardianTimestep& other) const
	{
		return GuardianTimestep(this->TimeDistance + other.TimeDistance);
	}

	const GuardianTimestep GuardianTimestep::operator+(const float timeDistance) const
	{
		return GuardianTimestep(this->TimeDistance + timeDistance);
	}

	const GuardianTimestep GuardianTimestep::operator-(const GuardianTimestep& other) const
	{
		return GuardianTimestep(this->TimeDistance - other.TimeDistance);
	}

	const GuardianTimestep GuardianTimestep::operator-(const float timeDistance) const
	{
		return GuardianTimestep(this->TimeDistance - timeDistance);
	}

	GuardianTimestep& GuardianTimestep::operator+=(const GuardianTimestep& other)
	{
		this->TimeDistance += other.TimeDistance;

		return *this;
	}

	GuardianTimestep& GuardianTimestep::operator+=(const float timeDistance)
	{
		this->TimeDistance += timeDistance;

		return *this;
	}

	GuardianTimestep& GuardianTimestep::operator-=(const GuardianTimestep& other)
	{
		this->TimeDistance -= other.TimeDistance;

		return *this;
	}

	GuardianTimestep& GuardianTimestep::operator-=(const float timeDistance)
	{
		this->TimeDistance -= timeDistance;

		return *this;
	}

	GuardianTimestep::operator float() const
	{
		return this->TimeDistance;
	}

	const float GuardianTimestep::GetSecond() const noexcept
	{
		return this->TimeDistance / 1000.0f;
	}

	const float GuardianTimestep::GetMillisecond() const noexcept
	{
		return this->TimeDistance;
	}
}
