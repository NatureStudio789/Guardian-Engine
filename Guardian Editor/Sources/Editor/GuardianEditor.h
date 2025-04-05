#ifndef _GE_GUARDIANEDITOR_H_
#define _GE_GUARDIANEDITOR_H_
#include "../Widget/Panel/GuardianPanel.h"

namespace GE
{
	class GuardianEditor
	{
	public:
		void AddWidgetToEditor(std::shared_ptr<GuardianWidget> widget);
		void ClearEditorWidgets();

		virtual void Initialize() {}

		virtual void Update();

		virtual void Render();

	protected:
		void RenderEditorWidgets();
		void UpdateEditorWidgets();

	private:
		std::vector<std::shared_ptr<GuardianWidget>> EditorWidgetList;
	};
}

#endif