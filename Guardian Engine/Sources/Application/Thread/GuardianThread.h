#ifndef _GE_GUARDIANTHREAD_H_
#define _GE_GUARDIANTHREAD_H_
#include "../../Profile/Time/GuardianTime.h"

namespace GE
{
	class GUARDIAN_API GuardianThread
	{
	public:
		GuardianThread();
		GuardianThread(const GuardianThread& ) = delete;
		GuardianThread(GuardianThread& other);
		~GuardianThread();

		template<typename FuncType, typename... Args>
		void LaunchThread(FuncType&& function, Args&&... args);

		void JoinThread();
		void DetachMainThread();

	private:
		std::thread ThreadObject;
	};

	template<typename FuncType, typename ...Args>
	inline void GuardianThread::LaunchThread(FuncType&& function, Args && ...args)
	{
		this->ThreadObject = std::thread(std::forward<FuncType>(function), std::forward<Args>(args)...);
	}
}

#endif