#ifndef _GE_GUARDIANPROGRAM_H_
#define _GE_GUARDIANPROGRAM_H_
#include "GuardianCreator.h"

namespace GE
{
	class GUARDIAN_API GuardianProgram
	{
	public:
		GuardianProgram()
		{
			this->ProgramName = "Guardian Program";
		}
		virtual ~GuardianProgram()
		{
			this->Release();
		}

		virtual void Initialize() {}
		virtual void Update() {}
		virtual void Release() {}

		const GString& GetProgramName() const noexcept;

	protected:
		GString ProgramName;

	private:
		virtual void Render() {}
		virtual LRESULT CALLBACK EditorMessageProcess(GWindowHandle hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { return 0; }

		friend class GuardianEngine;
		friend class GuardianApplication;
		friend class GuardianEditorEngine;
	};
}

#endif