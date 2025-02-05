#ifndef _GE_GUARDIANENGINE_H_
#define _GE_GUARDIANENGINE_H_
#include "Application/GuardianApplication.h"

namespace guardian
{
	class GUARDIAN_API GuardianEngine
	{
	public:
		GuardianEngine();
		GuardianEngine(const GuardianEngine&) = delete;
		~GuardianEngine();

		void InitializeEngine(GuardianProgram* program);

		void LaunchEngine();

		std::shared_ptr<GuardianScene> GetScene() noexcept;

		static std::unique_ptr<GuardianEngine> EngineInstance;

	private:
		GuardianProgram* EngineProgram;
		
		std::shared_ptr<GuardianScene> EngineScene;

		std::unique_ptr<GuardianEventProcesser> EngineEventProcesser;

		friend class GuardianEditor;
		friend class GuardianApplication;
	};
}

#endif