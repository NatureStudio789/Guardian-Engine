#ifndef _GE_GEDITOR_H_
#define _GE_GEDITOR_H_
#include "../Widget/Dockspace/GDockspace.h"

namespace GE
{
	namespace EUI
	{
		class GEditor
		{
		public:
			void AddWidgetToEditor(std::shared_ptr<GWidget> widget);
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
			std::vector<std::shared_ptr<GWidget>> EditorWidgetList;
		};
	}
}

#endif