#ifndef _GE_GUARDIANEDITOR_H_
#define _GE_GUARDIANEDITOR_H_
#include <Profile/EntryPoint/GuardianEntryPoint.h>

namespace guardian
{
	class GuardianEditor : public GuardianProgram
	{
	public:
		GuardianEditor();
		~GuardianEditor() override;

		void Initialize() override;
		void Update() override;
		void Render() override;
		void Release() override;

	private:
		void RenderDockspace();
		LRESULT CALLBACK EditorMessageProcess(GWindowHandle hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

		std::shared_ptr<GuardianModel> Nanosuit;
		std::shared_ptr<GuardianModel> Sponza;
	};
}

GUARDIAN_ENGINE_PROFILE(guardian::GuardianEditor)

#endif