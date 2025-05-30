#ifndef _GE_GUARDIANTIME_H_
#define _GE_GUARDIANTIME_H_
#include "Timer/GuardianTimer.h"

namespace GE
{
	class GUARDIAN_API GuardianTime
	{
	public:
		static void InitializeTime();

		static void UpdateTime();

		static const GString GetCurrentTimeExpression();
		static const GuardianTimestep& GetDeltaTime();
		static const int& GetFPSCount();

	private:
		static GuardianTimer FrameTimer;
		static GuardianTimestep DeltaTime;
		static int FPSCount;
	};
}

#endif