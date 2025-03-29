#ifndef _GE_GUARDIANEDITOR_H_
#define _GE_GUARDIANEDITOR_H_
#include "../Panel/GuardianPanel.h"
#include "../Panel/GuardianScenePanel.h"
#include "../Panel/GuardianSceneHierarchyPanel.h"
#include "../Panel/GuardianResourceBrowserPanel.h"
#include <Profile/EntryPoint/GuardianEntryPoint.h>

namespace GE
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
		void AddPanelToEditor(std::shared_ptr<GuardianPanel> panel);
		LRESULT CALLBACK EditorMessageProcess(GWindowHandle hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

		std::map<GString, std::shared_ptr<GuardianPanel>> EditorPanelList;
		std::shared_ptr<GuardianScenePanel> EditorScenePanel;
		std::shared_ptr<GuardianSceneHierarchyPanel> EditorSceneHierarchyPanel;
	};
}

GUARDIAN_ENGINE_PROFILE(GE::GuardianEditor)

#endif