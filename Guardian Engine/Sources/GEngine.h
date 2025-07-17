#ifndef _GE_GENGINE_H_
#define _GE_GENGINE_H_
#include "Profile/Program/GProgram.h"

namespace GE
{
	class GUARDIAN_API GEngine
	{
	public:
		GEngine();
		~GEngine();

		void InitializeEngine(std::shared_ptr<GProgram> program);
		void LaunchEngine();
		void ReleaseEngine();

		static std::shared_ptr<GEngine> Instance;

	private:
		std::shared_ptr<GProgram> EngineProgram;

		friend class GWindow;
	};
}

#endif