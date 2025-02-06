#ifndef _GE_GUARDIANTIMER_H_
#define _GE_GUARDIANTIMER_H_
#include "../Timestep/GuardianTimestep.h"

namespace guardian
{
	class GUARDIAN_API GuardianTimer
	{
	public:
		GuardianTimer();
		GuardianTimer(const GuardianTimer& other);

		void Restart();
		bool Start();
		bool Stop();

		const GuardianTimestep GetElapsedTime() const noexcept;

	private:
		bool IsRunning;

		std::chrono::time_point<std::chrono::steady_clock> StartPoint;
		std::chrono::time_point<std::chrono::steady_clock> StopPoint;
	};
}

#endif