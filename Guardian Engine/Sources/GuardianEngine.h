#ifndef _GE_GUARDIANENGINE_H_
#define _GE_GUARDIANENGINE_H_
#include "Application/GuardianApplication.h"

namespace GE
{
	class GUARDIAN_API GuardianEngine
	{
	public:
		enum Mode
		{
			GE_ENGINE_EDITOR,
			GE_ENGINE_RUNTIME
		};

	public:
		GuardianEngine();
		GuardianEngine(const GuardianEngine&) = delete;
		~GuardianEngine();

		void InitializeEngine(GuardianCreator* creator, GuardianProgram* program);

		void LaunchEngine();

		void SetCurrentScene(const GString& sceneName);
		std::shared_ptr<GuardianScene> GetScene() noexcept;

		static std::unique_ptr<GuardianEngine> EngineInstance;

	private:
		void LaunchCreator();

		GuardianCreator* EngineCreator;
		GuardianProgram* EngineProgram;
		
		std::shared_ptr<GuardianProject> EngineProject;

		std::unique_ptr<GuardianEventProcesser> EngineEventProcesser;

		bool FinishedInitialization;

		friend class GuardianEditorEngine;
		friend class GuardianApplication;
	};
}

#endif