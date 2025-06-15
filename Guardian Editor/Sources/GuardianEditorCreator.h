#ifndef _GE_GUARDIANEDITORCREATOR_H_
#define _GE_GUARDIANEDITORCREATOR_H_
#include "Panel/GuardianScenePanel.h"
#include "Panel/GuardianSceneHierarchyPanel.h"
#include "Panel/GuardianResourceBrowserPanel.h"
#include "Editor/GuardianRendererEditor.h"
#include <GuardianEngine.h>

namespace GE
{
	class GuardianEditorCreator : public GuardianCreator
	{
	public:
		GuardianEditorCreator();
		~GuardianEditorCreator() override;

		void Initialize() override;

		void Update() override;
		void Render() override;

		void Release() override;

	private:
		void AddEditorToCreator(std::shared_ptr<EUI::GuardianEditor> editor);
		void RenderDockspace();
		LRESULT CALLBACK EditorMessageProcess(GWindowHandle hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

		std::map<GString, std::shared_ptr<EUI::GuardianEditor>> CreatorEditorList;
	};
}

#endif