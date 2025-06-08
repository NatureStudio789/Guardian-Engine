#ifndef _GE_GUARDIANEDITORENGINE_H_
#define _GE_GUARDIANEDITORENGINE_H_
#include "Panel/GuardianScenePanel.h"
#include "Panel/GuardianSceneHierarchyPanel.h"
#include "Panel/GuardianResourceBrowserPanel.h"
#include "Editor/GuardianRendererEditor.h"
#include <Profile/EntryPoint/GuardianEntryPoint.h>

namespace GE
{
	class GuardianEditorEngine : public GuardianProgram
	{
	public:
		GuardianEditorEngine();
		~GuardianEditorEngine() override;

		void Initialize() override;

		void Update() override;
		void Render() override;
		
		void Release() override;
	
	private:
		void RenderDockspace();
		void AddStandardEditorToEngine(std::shared_ptr<EUI::GuardianEditor> editor);
		void AddCreatorEditorToEngine(std::shared_ptr<EUI::GuardianEditor> editor);
		void AddPanelToEditor(std::shared_ptr<EUI::GuardianPanel> panel);
		LRESULT CALLBACK EditorMessageProcess(GWindowHandle hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

		std::map<GString, std::shared_ptr<EUI::GuardianEditor>> CreatorEditorList;
		std::map<GString, std::shared_ptr<EUI::GuardianEditor>> StandardEditorList;

		std::map<GString, std::shared_ptr<EUI::GuardianPanel>> EditorPanelList;
		std::shared_ptr<GuardianScenePanel> EditorScenePanel;
		std::shared_ptr<GuardianSceneHierarchyPanel> EditorSceneHierarchyPanel;
	};
}

GUARDIAN_ENGINE_PROFILE(GE::GuardianEditorEngine)

#endif