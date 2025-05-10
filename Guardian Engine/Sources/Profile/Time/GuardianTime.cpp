#include "GuardianTime.h"

namespace GE
{
	GuardianTimer GuardianTime::FrameTimer;
	GuardianTimestep GuardianTime::DeltaTime = 0.0f;
	int GuardianTime::FPSCount = 0;


	void GuardianTime::InitializeTime()
	{
		FrameTimer.Start();
	}

	void GuardianTime::UpdateTime()
	{
		DeltaTime = FrameTimer.GetElapsedTime();
		FrameTimer.Restart();
		static float Timestep = 0.0f;
		Timestep += DeltaTime;
		static int FrameCounts = 0;
		FrameCounts++;
		if (Timestep >= 1000.0f)
		{
			Timestep = 0.0f;
			FPSCount = FrameCounts;
			FrameCounts = 0;
		}
	}

	const GString GuardianTime::GetCurrentTimeExpression()
	{
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

		std::stringstream TimeExprStream;
		TimeExprStream << "[" << SystemTime.wYear << " - " <<
			SystemTime.wMonth << " - " << SystemTime.wDay << " ";
		TimeExprStream << SystemTime.wHour << " : " << SystemTime.wMinute << " : " << SystemTime.wSecond << "]";

		return TimeExprStream.str();
	}

	const GuardianTimestep& GuardianTime::GetDeltaTime()
	{
		return DeltaTime;
	}

	const int& GuardianTime::GetFPSCount()
	{
		return FPSCount;
	}
}
