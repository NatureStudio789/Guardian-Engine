#ifndef _GE_GUARDIANPROGRAM_H_
#define _GE_GUARDIANPROGRAM_H_
#include "../../ECS/Entity/GuardianScriptableEntity.h"

namespace GE
{
	class GUARDIAN_API GuardianProgram
	{
	public:
		enum Mode
		{
			GE_NONE_MODE,
			GE_STANDARD_MODE,
			GE_CREATOR_MODE
		};

	public:
		GuardianProgram()
		{
			this->ProgramName = "Guardian Program";
			this->ProgramMode = GE_NONE_MODE;
			this->IsProgramExecuted = false;
		}
		virtual ~GuardianProgram()
		{
			this->Release();
			this->ProgramMode = GE_NONE_MODE;
			this->IsProgramExecuted = false;
		}

		virtual void Initialize() {}
		virtual void Update() {}
		virtual void Release() {}

		void LaunchProgram();
		void ShutdownProgram();
		bool IsProgramRunning() const noexcept;

		void SetProgramMode(const Mode& mode);
		const Mode& GetProgramMode(const Mode& mode) const noexcept;

		const GString& GetProgramName() const noexcept;

	protected:
		GString ProgramName;
		Mode ProgramMode;
		bool IsProgramExecuted;

	private:
		virtual void Render() {}
		virtual LRESULT CALLBACK EditorMessageProcess(GWindowHandle hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { return 0; }

		friend class GuardianEngine;
		friend class GuardianApplication;
		friend class GuardianEditorEngine;
	};
}

#endif