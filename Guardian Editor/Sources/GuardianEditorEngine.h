#ifndef _GE_GUARDIANEDITORENGINE_H_
#define _GE_GUARDIANEDITORENGINE_H_
#include "Widget/Panel/GuardianPanel.h"
#include "Widget/Panel/GuardianScenePanel.h"
#include "Widget/Panel/GuardianSceneHierarchyPanel.h"
#include "Widget/Panel/GuardianResourceBrowserPanel.h"
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
		void AddEditorToEngine(std::shared_ptr<GuardianEditor> editor);
		void AddPanelToEditor(std::shared_ptr<GuardianPanel> panel);
		LRESULT CALLBACK EditorMessageProcess(GWindowHandle hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

		std::vector<std::shared_ptr<GuardianEditor>> EngineEditorList;

		std::map<GString, std::shared_ptr<GuardianPanel>> EditorPanelList;
		std::shared_ptr<GuardianScenePanel> EditorScenePanel;
		std::shared_ptr<GuardianSceneHierarchyPanel> EditorSceneHierarchyPanel;
	};
}

GUARDIAN_ENGINE_PROFILE(GE::GuardianEditorEngine)

#endif