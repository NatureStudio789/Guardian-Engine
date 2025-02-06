#include "GuardianTimer.h"

namespace guardian
{
	GuardianTimer::GuardianTimer()
	{
		this->StartPoint = std::chrono::high_resolution_clock::now();
		this->StopPoint = std::chrono::high_resolution_clock::now();
		this->IsRunning = false;
	}

	GuardianTimer::GuardianTimer(const GuardianTimer& other)
	{
		this->IsRunning = other.IsRunning;
		this->StartPoint = other.StartPoint;
		this->StopPoint = other.StopPoint;
	}

	void GuardianTimer::Restart()
	{
		this->IsRunning = true;
		this->StartPoint = std::chrono::high_resolution_clock::now();
	}

	bool GuardianTimer::Start()
	{
		if (this->IsRunning)
		{
			return false;
		}
		else
		{
			this->StartPoint = std::chrono::high_resolution_clock::now();
			this->IsRunning = true;
			return true;
		}
	}

	bool GuardianTimer::Stop()
	{
		if (!this->IsRunning)
		{
			return false;
		}
		else
		{
			this->StopPoint = std::chrono::high_resolution_clock::now();
			this->IsRunning = false;
			return true;
		}
	}

	const GuardianTimestep GuardianTimer::GetElapsedTime() const noexcept
	{
		if (this->IsRunning)
		{
			auto elapsed = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - this->StartPoint).count();

			return elapsed;
		}
		else
		{
			auto elapsed = std::chrono::duration<float, std::milli>(this->StopPoint - this->StartPoint).count();

			return elapsed;
		}
	}
}
