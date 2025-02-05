#include "GuardianTimepoint.h"

namespace guardian
{
	GuardianTimepoint::GuardianTimepoint()
	{
		this->Timepoint = 0.0f;
	}

	GuardianTimepoint::GuardianTimepoint(float timePoint)
	{
		this->Timepoint = timePoint;
	}

	GuardianTimepoint::GuardianTimepoint(const GuardianTimepoint& other)
	{
		this->Timepoint = other.Timepoint;
	}

	const float GuardianTimepoint::operator-(const GuardianTimepoint& other) const
	{
		return this->Timepoint - other.Timepoint;
	}

	const float GuardianTimepoint::operator-(const float timePoint) const
	{
		return this->Timepoint - timePoint;
	}

	const float GuardianTimepoint::GetMillisecond() const noexcept
	{
		return this->Timepoint;
	}

	const float GuardianTimepoint::GetSecond() const noexcept
	{
		return this->Timepoint / 1000.0f;
	}

	const GuardianTimepoint GuardianTimepoint::GetCurrentTimepoint() noexcept
	{
		return GuardianTimepoint((float)std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count());
	}
}
