#ifndef _GE_GUARDIANEDITOR_H_
#define _GE_GUARDIANEDITOR_H_
#include "../Widget/Panel/GuardianPanel.h"

namespace GE
{
	namespace EUI
	{
		class GuardianEditor
		{
		public:
			void AddWidgetToEditor(std::shared_ptr<GuardianWidget> widget);
			void ClearEditorWidgets();

			virtual void Initialize() {}

			virtual void Update();

			virtual void Render();

			const std::string& GetEditorName() const noexcept;

		protected:
			std::string EditorName;

			void RenderEditorWidgets();
			void UpdateEditorWidgets();

		private:
			std::vector<std::shared_ptr<GuardianWidget>> EditorWidgetList;
		};
	}
}

#endif